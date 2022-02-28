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
Implement multiservice Server entity.

A Server is a kind of entity in multiservice interface that waits for client requests
and response it is available, blocking the Server to other requests.
If the Client has targeted this entity, it will wait for the data and will answer with the
solution get from a routine.
If the Client targets other Server, this Server will unblock for other Client requests.
"""

from amlip_nodes.dds.entity.AmlParticipant import AmlParticipant
from amlip_nodes.dds.entity.AmlReader import AmlReader
from amlip_nodes.dds.entity.AmlTopic import AmlTopic
from amlip_nodes.dds.entity.AmlWriter import AmlWriter
from amlip_nodes.dds.node.AmlNodeId import AmlNodeId
from amlip_nodes.exception.Exception import InconsistencyException, TimeoutException
from amlip_nodes.log.log import logger

MAX_TIMEOUT_DATA_MSSERVER = 10


class AmlMultiserviceServer():
    """
    Multiservice Server entity.

    Create a Server entity with every DDS subentity required.
    """

    def __init__(
            self,
            node_id: AmlNodeId,
            service_name: str,
            aml_participant: AmlParticipant,
            aml_reader_request_availability_topic: AmlTopic,
            aml_writer_reply_available_topic: AmlTopic,
            aml_reader_task_target_topic: AmlTopic,
            aml_reader_task_topic: AmlTopic,
            aml_writer_solution_topic: AmlTopic,
            callback=None):
        """
        Create a AmlMultiserviceServer entity.

        :param callback: routine that will be called when processing a request.
            If callback is None, it will require to specify the callback when calling
            process request.
        """
        logger.construct(f'Creating AmlMultiserviceServer {service_name}.')

        # Internal elements
        self.node_id_ = node_id
        self.name_ = service_name
        self.callback_ = callback
        # This list collects all task that clients has already set as chosen
        # so this server does not try to use them
        self.tasks_already_taken_ = []

        # Create request availability Reader
        self.aml_reader_request_availability_ = AmlReader(
            aml_topic=aml_reader_request_availability_topic,
            aml_participant=aml_participant)

        # Create reply available Writer
        self.aml_writer_reply_available_ = AmlWriter(
            aml_topic=aml_writer_reply_available_topic,
            aml_participant=aml_participant)

        # Create request task target Reader
        self.aml_reader_task_target_ = AmlReader(
            aml_topic=aml_reader_task_target_topic,
            aml_participant=aml_participant)

        # Create request task Reader
        self.aml_reader_task_ = AmlReader(
            aml_topic=aml_reader_task_topic,
            aml_participant=aml_participant)

        # Create reply solution Writer
        self.aml_writer_solution_ = AmlWriter(
            aml_topic=aml_writer_solution_topic,
            aml_participant=aml_participant)

    def __del__(self):
        """Stop entity and destroy it."""
        logger.construct(f'Deleting AmlMultiserviceServer {self.name_}.')
        self.stop()

    def stop(self):
        """
        Set entity as stopped and stop every internal reader.

        If it is already processing the callback, it will wait
        till finished.
        """
        # Set as stopped
        self.stop_ = True

        # Stop all readers
        self.aml_reader_request_availability_.stop()
        self.aml_reader_task_target_.stop()
        self.aml_reader_task_.stop()

    def process_request(self, callback=None) -> bool:
        """
        Process client requests messages.

        - Wait till a Client ask for an available Server
        - Send availability message to that Client
        - Wait for Task Target message from Client
            - If Client targets other Server, exit with False
            - If Client targets this Server, wait for data
        - Send solution to the data received

        :return: True if request has been processed,
        :return: False if the request has been targeted to other Server or failed.

        :raise StopException: if entity is stopped while in this routine.
        """
        # If callback is none, use this object callback
        if callback is None:
            if self.callback_ is None:
                raise InconsistencyException('Process request needs a valid callback.')
            else:
                callback = self.callback_

        # Wait for an availability request and get the client and task ids when one is available
        request = self._wait_request_availability()
        requester_id, task_id = request.requester_id(), request.task_id()

        # If the task has been already processed as chosen, skip it
        # Note: this could be done as well in _wait_request_availability
        if (requester_id, task_id) in self.tasks_already_taken_:
            return False

        # Send that this server is available
        self._send_reply_available(requester_id, task_id)

        # Wait for the answer of the client, that could be positive or negative
        if not self._wait_task_target(requester_id, task_id):
            # This is not the server for the client, finish routine
            return False

        # This is the server chose, so wait for the data from the client
        data_received = self._wait_task(requester_id, task_id)

        # Send solution
        self._send_solution(requester_id, task_id, data_received.data())

        # Request has been processed
        return True

    def _wait_request_availability(self):
        """
        Wait till a client requests a server.

        Return id of the client and the task available.
        """
        while True:
            # Wait for a message
            self.aml_reader_request_availability_.wait_to_data_receive(MAX_TIMEOUT_DATA_MSSERVER)

            # Once a message is get, send that this server is available
            request_received = self.aml_reader_request_availability_.read()
            # WARNING: do not return only the data because this object will be destroyed
            return request_received

    def _send_reply_available(self, requester_id, task_id):
        """Create a reply data to a client that is asking."""
        # Get data and fill it
        reply_data = self.aml_writer_reply_available_.new_data()
        reply_data.requester_id(requester_id)
        reply_data.task_id(task_id)
        reply_data.server_id(self.node_id_.id_str())

        # Send message
        self.aml_writer_reply_available_.write(reply_data)

    def _wait_task_target(self, requester_id, task_id):
        """
        Wait till the client replies if this is the server chosen.

        return: True if this server has been selected, False otherwise.
        """
        while True:
            # Wait for a message
            try:
                self.aml_reader_task_target_.wait_to_data_receive(MAX_TIMEOUT_DATA_MSSERVER)
            except TimeoutException:
                # It may desynchronize whit other thread, try again
                continue

            # Get every message till a server answering this id task and client
            while self.aml_reader_task_target_.data_available():
                task_target_received = self.aml_reader_task_target_.read()
                if task_target_received.requester_id() != requester_id:
                    # Reply from different Client, store it and skip
                    self.tasks_already_taken_.append(
                        (task_target_received.requester_id(), task_target_received.task_id()))
                    continue

                elif task_target_received.task_id() != task_id:
                    # Reply for different Task, store it and skip
                    self.tasks_already_taken_.append(
                        (task_target_received.requester_id(), task_target_received.task_id()))
                    continue

                else:
                    # Client has answered, return if this server is the target
                    if task_target_received.server_id() == self.node_id_.id_str():
                        return True
                    else:
                        return False

    def _wait_task(self, requester_id, task_id):
        """
        Wait for the client source to send the task.

        return: Data with the task received
        """
        while True:
            # Wait for a message
            try:
                self.aml_reader_task_.wait_to_data_receive(MAX_TIMEOUT_DATA_MSSERVER)
            except TimeoutException:
                # It may desynchronize whit other thread, try again
                continue

            # Get every message till a server answering this id task and client
            while self.aml_reader_task_.data_available():
                task_received = self.aml_reader_task_.read()
                if task_received.task_reference().requester_id() != requester_id:
                    # Reply from different Client, skip
                    continue
                elif task_received.task_reference().task_id() != task_id:
                    # Reply for different Task, skip
                    continue
                elif task_received.task_reference().server_id() != self.node_id_.id_str():
                    # Task send to a different Server even this is the target
                    # This should not happen!
                    raise InconsistencyException('Task targeted to this server is sent to other.')
                else:
                    # Get data from task message
                    # WARNING: do not return only the data() because this object will be destroyed
                    return task_received

    def _send_solution(self, requester_id, task_id, task_data):
        """
        Send solution to request.

        Create the solution for the data that has been received.
        Calculate the solution.
        Send the solution to the Client.
        """
        # Get solution to the task
        solution = self.callback_(task_data)

        # Get data and fill request reference
        solution_data = self.aml_writer_solution_.new_data()
        task_reference_ = solution_data.task_reference()
        task_reference_.requester_id(requester_id)
        task_reference_.task_id(task_id)
        task_reference_.server_id(self.node_id_.id_str())
        solution_data.task_reference(task_reference_)

        # Store solution
        solution_data.data(solution)

        # Send message
        self.aml_writer_solution_.write(solution_data)
