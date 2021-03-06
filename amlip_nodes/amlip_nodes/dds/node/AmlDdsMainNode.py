# Copyright 2022 Proyectos y Sistemas de Mantenimiento SL (eProsima).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""
Computational Node Participant.

DDS Participant to AML Computational Node.
It gets Jobs, consumed them and return an atomization.
"""

import threading
from enum import Enum

import amlip_nodes.dds.network.aml_topic_names as topic_names
from amlip_nodes.aml.aml_types import Job, JobSolution
from amlip_nodes.dds.node.AmlDdsNode import AmlDdsNode
from amlip_nodes.exception.Exception import \
    InconsistencyException, NoDataException, StopException, TimeoutException
from amlip_nodes.log.log import logger

import inference

import job

import status


class AsyncCallStatus(Enum):
    """Async client call status."""

    RUNNING = 0
    STOPPED = 1
    FINISHED = 2
    ALREADY_READ = 3
    TIMEOUT = 4


class AmlDdsMainNode(AmlDdsNode):
    """TODO comment."""

    def __init__(self,
                 name,
                 inference_process_callback=None,
                 domain=0):
        """Construct MainNodeParticipant object."""
        super().__init__(name, domain)

        logger.construct(f'Constructing AmlDdsMainNode {name}')

        # Internal values
        self.inference_server_ = None
        self.job_client_ = None
        self.inference_process_callback_ = inference_process_callback

        # Async threads
        self.async_job_requests_ = {}

    def init(self):
        """TODO comment."""
        # Main Node init
        super().init()

        # Create Inference Server
        self.inference_server_ = self._create_multiservice_server(
            service_name=topic_names.INFERENCE_SERVICE_NAME,
            topic_data_type_data_pubsub_constructor=inference.Inference_TaskPubSubType,
            topic_data_type_data_constructor=inference.Inference_Task,
            topic_data_type_solution_pubsub_constructor=inference.Inference_SolutionPubSubType,
            topic_data_type_solution_constructor=inference.Inference_Solution,
            callback=self.inference_process_callback_)

        # Create Job Client
        self.job_client_ = self._create_multiservice_client(
            service_name=topic_names.JOB_SERVICE_NAME,
            topic_data_type_data_pubsub_constructor=job.Job_TaskPubSubType,
            topic_data_type_data_constructor=job.Job_Task,
            topic_data_type_solution_pubsub_constructor=job.Job_SolutionPubSubType,
            topic_data_type_solution_constructor=job.Job_Solution)

    def _node_kind(self):
        """TODO comment."""
        return status.MAIN

    def _inference_callback(self, data):
        """TODO."""
        pass

    def stop(self):
        """Stop internal interfaces."""
        self.inference_server_.stop()
        self.job_client_.stop()

    def is_job_answered(
            self,
            job: Job):
        """Check if the job with this index has been answered already."""
        if job.index in self.async_job_requests_.keys():
            return (self.async_job_requests_[job.index]['status'] == AsyncCallStatus.FINISHED or
                    self.async_job_requests_[job.index]['status'] == AsyncCallStatus.ALREADY_READ)
        return False

    def is_job_timeout(
            self,
            job: Job):
        """Check if the job with this index has been closed by timeout."""
        if job.index in self.async_job_requests_.keys():
            return self.async_job_requests_[job.index]['status'] == AsyncCallStatus.TIMEOUT
        return False

    def is_job_already_read(
            self,
            job: Job):
        """Check if the job with this index has been answered already."""
        if job.index in self.async_job_requests_.keys():
            return self.async_job_requests_[job.index]['status'] == AsyncCallStatus.ALREADY_READ
        return False

    def get_job_response(
            self,
            job: Job):
        """Check if the job with this index has been answered already."""
        if not self.is_job_answered(job):
            raise NoDataException('')
        else:
            if not self.is_job_already_read(job):
                # In case the job has finished but it has not been read yet, "read it"
                # This means to join the thread so it is not left orphan
                self.async_job_requests_[job.index]['thread'].join()

            # Return solution
            return (self.async_job_requests_[job.index]['solution'])

    def send_job_async(
            self,
            job: Job):
        """
        Send one request in a new thread.

        This threads and the solution for each job is stored in an internal variable
        """
        if job.index in self.async_job_requests_.keys():
            # The job has been already sent, This should not happen!
            raise InconsistencyException(f'Job {job.index} has already been sent.')

        # Create this job in job list
        self.async_job_requests_[job.index] = {}

        # Create thread and store it
        request_thread = threading.Thread(target=self._send_job_async_thread_routine, args=(job,))
        self.async_job_requests_[job.index]['thread'] = request_thread
        self.async_job_requests_[job.index]['status'] = AsyncCallStatus.RUNNING

        request_thread.start()

    def _send_job_async_thread_routine(
            self,
            job: Job):
        """Routine to exectue an async request job."""
        try:
            # Send the job and wait for answer
            solution = self.job_client_.send_request(
                data=Job.to_dds_data_type(job),
                task_id=job.index)

            logger.execution(f'{self.name_} got solution for job {job.index}')

            # Store solution
            self.async_job_requests_[job.index]['solution'] = JobSolution.from_dds_data_type(solution.data())
            self.async_job_requests_[job.index]['status'] = AsyncCallStatus.FINISHED
            self.async_job_requests_[job.index]['server'] = solution.task_reference().server_id()

        except StopException:
            # Stopped before the answer arrives, set it as stopped
            self.async_job_requests_[job.index]['status'] = AsyncCallStatus.STOPPED

        except TimeoutException:
            # Some of the messages has not been answered in enough time
            self.async_job_requests_[job.index]['status'] = AsyncCallStatus.TIMEOUT

    def task_timeout(self, task_id):
        """Set a task as timeout and send a target message that is no longer available."""
        self.job_client_.send_task_target_timeout(task_id)
