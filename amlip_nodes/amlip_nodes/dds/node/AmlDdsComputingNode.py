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
Computing Node Participant.

DDS Participant to AML Computing Node.
It gets Jobs, consumed them and return an atomization.
"""

import amlip_nodes.dds.network.aml_topic_names as topic_names
from amlip_nodes.dds.node.AmlDdsNode import AmlDdsNode

import job

import status


class AmlDdsComputingNode(AmlDdsNode):
    """TODO comment."""

    def __init__(self,
                 name,
                 job_process_callback,
                 domain=0):
        """Construct MainNodeParticipant object."""
        super().__init__(name, domain)

        # Internal values
        self.job_server_ = None
        self.job_process_callback_ = job_process_callback

    def init(self):
        """TODO comment."""
        # Ai Node init
        super().init()

        # Create Job Server
        self.job_server_ = self._create_multiservice_server(
            service_name=topic_names.JOB_SERVICE_NAME,
            topic_data_type_data_pubsub_constructor=job.Job_TaskPubSubType,
            topic_data_type_data_constructor=job.Job_Task,
            topic_data_type_solution_pubsub_constructor=job.Job_SolutionPubSubType,
            topic_data_type_solution_constructor=job.Job_Solution,
            callback=self.job_process_callback_)

    def _node_kind(self):
        """TODO comment."""
        return status.COMPUTATIONAL

    def stop(self):
        """Stop internal interfaces."""
        self.job_server_.stop()

    def process_job(self):
        """Process first job that arrives."""
        # Try to process a request until one client accept it and process it
        while not self.job_server_.process_request():
            pass
