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
Implement pubsub Reader entity.

This kind of entity is able to Read DDS messages and to wait for data to read.
It handles a DDS DataReader and a Subscriber.
"""

from threading import Condition
from time import sleep

from amlip_nodes.dds.entity.AmlParticipant import AmlParticipant
from amlip_nodes.dds.entity.AmlTopic import AmlTopic
from amlip_nodes.exception.Exception import NoDataException, StopException, TimeoutException
from amlip_nodes.log.log import logger

import fastdds


class AmlReader(fastdds.DataReaderListener):
    """
    AmlReader entity.

    This class is its own Listener.
    Every message that arrives is taken from reader in the callback and
    is stored in a list of data so the user can access it afterwards.
    """

    def __init__(
            self,
            aml_topic: AmlTopic,
            aml_participant: AmlParticipant,
            subscriber_qos=fastdds.SubscriberQos(),
            datareader_qos=None):
        """
        Create an AmlReader entity.

        It creates a DDS Subscriber and inside a DDS DataReader.
        """
        super().__init__()

        if datareader_qos is None:
            datareader_qos = AmlReader.default_datareader_qos()

        logger.construct(f'Creating AmlReader {aml_topic.name()}, {aml_topic.type_name()}.')

        # Topic
        self.aml_topic_ = aml_topic
        # Listener
        self.msgs_to_read_ = []
        # Wait Condition
        self.cv_wait_data_ = Condition()
        self.stop_ = False

        # Create Subscriber
        self.subscriber_ = aml_participant.get_dds_participant().create_subscriber(
                subscriber_qos)

        # Create DataReader
        self.reader_ = self.subscriber_.create_datareader(
            aml_topic.dds_topic(),
            datareader_qos,
            self)

        # Wait for a minimum time so the entity is created and discovered
        sleep(0.2)

    def on_data_available(self, reader):
        """
        Notify when a new data arrives to DataReader.

        Overload from DataReaderListener method.
        Awake any thread waiting for messages.
        """
        # Get Condition in wait
        self.cv_wait_data_.acquire()

        # Read data and store it
        info = fastdds.SampleInfo()
        data = self.aml_topic_.new_object()
        reader.take_next_sample(data, info)

        self.msgs_to_read_.append(data)

        # Notify in Condition in wait and release it
        self.cv_wait_data_.notify_all()
        self.cv_wait_data_.release()

    def stop(self):
        """Set entity as stopped and awake every thread waiting for messages."""

        # Get Condition in wait, notify in Condition in wait and release it
        self.cv_wait_data_.acquire()
        # Set object as stopped
        self.stop_ = True
        self.cv_wait_data_.notify_all()
        self.cv_wait_data_.release()

    def read(self):
        """Get next data available and remove it from received data."""
        if self.data_available():
            return self.msgs_to_read_.pop(0)

        else:
            # No data available to read
            raise NoDataException('Ask for data in an empty Reader.')

    def data_available(self) -> bool:
        """Return True if there is data available to read, False otherwise."""
        return len(self.msgs_to_read_) > 0

    def wait_to_data_receive(self, timeout=None):
        """
        Wait for data to arrive to Reader.

        It stops the thread (passive wait) until new data is available in the Reader.

        :raise StopException if the wait has been cancelled due to a stop.
        """
        self.cv_wait_data_.acquire()
        self.cv_wait_data_.wait_for(
            predicate=lambda:
                self.data_available() or self.stop_,
            timeout=timeout)
        self.cv_wait_data_.release()

        # If awake due to a stop
        if self.stop_:
            raise StopException('Stopped Reader while waiting.')

        # If awake due to no data availble => timeout
        if not self.data_available():
            raise TimeoutException('Timeout waiting data.')

    @staticmethod
    def default_datareader_qos():
        """Return default DataReader QoS for this project."""
        rqos = fastdds.DataReaderQos()

        # Transient
        rqos.durability().kind = fastdds.TRANSIENT_LOCAL_DURABILITY_QOS
        rqos.history().kind = fastdds.KEEP_ALL_HISTORY_QOS
        rqos.history().depth = 1000
        # Reliable
        rqos.reliability().kind = fastdds.RELIABLE_RELIABILITY_QOS

        return rqos
