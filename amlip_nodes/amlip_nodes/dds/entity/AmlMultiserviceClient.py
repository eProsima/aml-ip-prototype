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
Implement multiservice Client entity.

A Client is a kind of entity in multiservice interface that send request publicly
and wait for a server to reply that is available.
Once a free server has replied, it sends the data and wait for the response.
"""

from amlip_nodes.dds.entity.AmlParticipant import AmlParticipant
from amlip_nodes.dds.entity.AmlReader import AmlReader
from amlip_nodes.dds.entity.AmlTopic import AmlTopic
from amlip_nodes.dds.entity.AmlWriter import AmlWriter
from amlip_nodes.dds.node.AmlNodeId import AmlNodeId
from amlip_nodes.exception.Exception import InconsistencyException

MAX_TIMEOUT_DATA_MSCLIENT = 10


class AmlMultiserviceClient():
    """Multiservice Client entity."""

    def __init__(
            self,
            node_id: AmlNodeId,
            service_name: str,
            aml_participant: AmlParticipant,
            aml_writer_request_availability_topic: AmlTopic,
            aml_reader_reply_available_topic: AmlTopic,
            aml_writer_task_target_topic: AmlTopic,
            aml_writer_task_topic: AmlTopic,
            aml_reader_solution_topic: AmlTopic):
        """
        Create a AmlMultiserviceClient entity.

        Create a Client entity with every DDS subentity required.
        """
        print(f'Creating AmlMultiserviceClient {service_name}.')
        # Internal elements
        self.node_id_ = node_id
        self.name_ = service_name
        self.request_number_ = 0
        self.stop_ = False
        self.task_id_ = 0
        # This list collects all task that servers has set as available to
        self.request_already_available_ = {}

        # Create request availability Writer
        self.aml_writer_request_availability_ = AmlWriter(
            aml_topic=aml_writer_request_availability_topic,
            aml_participant=aml_participant)

        # Create reply available Reader
        self.aml_reader_reply_available_ = AmlReader(
            aml_topic=aml_reader_reply_available_topic,
            aml_participant=aml_participant)

        # Create task target Writer
        self.aml_writer_task_target_ = AmlWriter(
            aml_topic=aml_writer_task_target_topic,
            aml_participant=aml_participant)

        # Create task Writer
        self.aml_writer_task_ = AmlWriter(
            aml_topic=aml_writer_task_topic,
            aml_participant=aml_participant)

        # Create solution Reader
        self.aml_reader_solution_ = AmlReader(
            aml_topic=aml_reader_solution_topic,
            aml_participant=aml_participant)

    def __del__(self):
        """Stop entity and destroy it."""
        self.stop()

    def stop(self):
        """Set entity as stopped and stop every internal reader."""
        # Set as stopped
        self.stop_ = True

        # Stop all readers
        self.aml_reader_reply_available_.stop()
        self.aml_reader_solution_.stop()

    def send_request(self, data):
        """
        Send a request and wait response.

        - Send a request of availability.
        - Wait for an available server to reply.
        - Once a server has been found, publish that this will be the target server.
        - Send data to the target server.
        - Wait for the solution to the task.

        :return: Solution from the server

        :raise StopException: if entity is stopped while in this routine.
        """
        # Create new Task id
        task_id = self.task_id_
        self.task_id_ += 1

        # First send a service request
        self._send_request_availability(task_id)

        # Then wait for the answer from the first server
        data_reply_available = self._wait_reply_available(task_id)
        server_id = data_reply_available.server_id()

        # Once a server is ready, publish which is the server target and
        # send it the data
        self._send_task_target(task_id, server_id)
        self._send_task(task_id, server_id, data)

        # Wait for the answer of this server
        response = self._wait_solution(task_id, server_id)

        # Finish process and retrieve data
        return response

    def _send_request_availability(self, task_id):
        """Create a request data and send it through specific writer."""
        # Get data and fill it
        request_data = self.aml_writer_request_availability_.new_data()
        request_data.requester_id(self.node_id_.id_str())
        request_data.task_id(task_id)

        # Send message
        self.aml_writer_request_availability_.write(request_data)

    def _wait_reply_available(self, task_id):
        """
        Wait till a server replies to the request.

        Return id of the server that will answer the request.
        """
        while True:
            # Check if there is already an answer for this task
            if task_id in self.request_already_available_.keys():
                return self.request_already_available_[task_id]

            # Wait for a message
            self.aml_reader_reply_available_.wait_to_data_receive(MAX_TIMEOUT_DATA_MSCLIENT)

            # Get every message till a server answering this id task and client
            while self.aml_reader_reply_available_.data_available():
                reply_received = self.aml_reader_reply_available_.read()
                if reply_received.requester_id() != self.node_id_.id_str():
                    # Reply for different Client, skip
                    continue

                elif reply_received.task_id() != task_id:
                    # Store it so other threads know it has arrived and skip
                    self.request_already_available_[task_id] = reply_received
                    continue

                else:
                    # Server has answered, return ID of server
                    # Store this server so it is not
                    # WARNING do not return server_id because this data will be removed
                    self.request_already_available_[task_id] = reply_received
                    return reply_received

    def _send_task_target(self, task_id, server_id):
        """Send task target data for all servers."""
        # Get new data and fill it with the server that will work
        task_target = self.aml_writer_task_target_.new_data()
        task_target.requester_id(self.node_id_.id_str())
        task_target.task_id(task_id)
        task_target.server_id(server_id)

        # Send message
        self.aml_writer_task_target_.write(task_target)

    def _send_task(self, task_id, server_id, data):
        """Send data for the server that has answered."""
        # Get new data type to send
        task_data = self.aml_writer_task_.new_data()
        # Fill Request reference
        task_reference_ = task_data.task_reference()
        task_reference_.requester_id(self.node_id_.id_str())
        task_reference_.task_id(task_id)
        task_reference_.server_id(server_id)
        task_data.task_reference(task_reference_)

        # Fill data
        task_data.data(data)

        # Send message
        self.aml_writer_task_.write(task_data)

    def _wait_solution(self, task_id, server_id):
        """
        Wait till a server replies to the request.

        Return id of the server that will answer the request.
        """
        while True:
            # Wait for a message
            self.aml_reader_solution_.wait_to_data_receive(MAX_TIMEOUT_DATA_MSCLIENT)

            # Get every message till a server answering this id is listen
            while self.aml_reader_solution_.data_available():
                solution_received = self.aml_reader_solution_.read()
                solution_received_reference = solution_received.task_reference()

                if solution_received_reference.requester_id() != self.node_id_.id_str():
                    # Reply for different Client, skip
                    continue
                elif solution_received_reference.task_id() != task_id:
                    # Reply for old request, skip
                    continue
                elif solution_received_reference.server_id() != server_id:
                    # Reply from other server. This should not happen!
                    raise InconsistencyException('Task answered from a not corresponding server.')
                else:
                    # Server has answered, return solution
                    # WARNING do not return data because this data will be removed
                    return solution_received
