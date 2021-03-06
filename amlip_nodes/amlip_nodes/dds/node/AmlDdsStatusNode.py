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
Status Node Participant.

DDS Participant to AML Status Node.
It gets Jobs, consumed them and return an atomization.
"""

from datetime import datetime

import amlip_nodes.dds.network.aml_topic_names as topic_names
from amlip_nodes.dds.node.AmlDdsNode import AmlDdsNode
from amlip_nodes.exception.Exception import StopException
from amlip_nodes.log.log import logger

import status


class AmlDdsStatusNode(AmlDdsNode):
    """TODO comment."""

    def __init__(self,
                 name,
                 domain=0):
        """Construct StatusNodeParticipant object."""
        super().__init__(name, domain)

        # Internal values
        self.status_reader_ = None
        self.status_history_ = []

        # Stop variables
        self.stop_ = False

        # Routine thread
        self.routine_thread_ = None

    def init(self):
        """TODO comment."""
        # Ai Node init
        super().init()

        # Create Status Reader
        self.status_reader_ = self._create_reader(
            topic_name=topic_names.STATUS_TOPIC_NAME,
            topic_data_type_pubsub_constructor=status.StatusPubSubType,
            topic_data_type_constructor=status.Status)

    def __del__(self):
        """Store in file the status read."""

    def _node_kind(self):
        """TODO comment."""
        return status.STATUS

    def stop(self):
        """Stop internal interfaces."""
        self.stop_ = True
        self.status_reader_.stop()

    def process_status_async_routine(self):
        """Process first job that arrives."""
        # Try to process a request until one client accept it and process it
        while self._process_status():
            pass

    def _process_status(self):
        """Process first job that arrives."""
        try:
            # Wait till there are messages to read
            self.status_reader_.wait_to_data_receive()

            # Check there is data available
            if self.status_reader_.data_available():
                # Read data
                status_data = self.status_reader_.read()

                # Store and Print data
                self.status_history_.append((datetime.now().strftime("%H:%M:%S"), status_data))
                logger.user(
                    f'\n{self.name_} has read data message:\n' +
                    AmlDdsStatusNode._str_status_data(status_data))

                return True

            else:
                # There is no data and the thread has awake, exit
                return False

        except StopException:
            return False

    @staticmethod
    def _str_status_data(data) -> str:
        """Print status data arrived from other node."""
        return (
            f'  ID          : {data.id()}\n'
            f'  NAME        : {data.name()}\n'
            f'  NODE KIND   : {AmlDdsStatusNode.__str_to_status_node_kind(data)}\n'
            f'  STATUS      : {AmlDdsStatusNode.__str_to_status_status(data)}\n'
        )

    @staticmethod
    def __str_to_status_node_kind(data) -> str:
        """TODO comment."""
        if data.node_kind() == status.MAIN:
            return 'MainNode'
        elif data.node_kind() == status.COMPUTATIONAL:
            return 'ComputingNode'
        elif data.node_kind() == status.STATUS:
            return 'Status'

    @staticmethod
    def __str_to_status_status(data) -> str:
        """TODO comment."""
        if data.status() == status.RUNNING:
            return 'running'
        elif data.status() == status.DISABLED:
            return 'stopped'
