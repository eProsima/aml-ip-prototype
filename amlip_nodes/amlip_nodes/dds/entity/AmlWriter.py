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
Implement pubsub Writer entity.

This kind of entity is able to Write DDS messages.
It handles a DDS DataWriter and a Publisher.
"""

from time import sleep

from amlip_nodes.dds.entity.AmlParticipant import AmlParticipant
from amlip_nodes.dds.entity.AmlTopic import AmlTopic
from amlip_nodes.log.log import logger

import fastdds


class AmlWriter():
    """AmlWriter entity."""

    def __init__(
            self,
            aml_topic: AmlTopic,
            aml_participant: AmlParticipant,
            publisher_qos=fastdds.PublisherQos(),
            datawriter_qos=fastdds.DataWriterQos()):
        """
        Create an AmlWriter entity.

        It creates a DDS Publisher and inside a DDS DataWriter.
        """
        logger.construct(f'Creating AmlWriter {aml_topic.name()}, {aml_topic.type_name()}.')

        # Topic
        self.aml_topic_ = aml_topic

        # Create Publisher
        self.publisher_ = aml_participant.get_dds_participant().create_publisher(
                publisher_qos)

        # Create DataWriter
        self.writer_ = self.publisher_.create_datawriter(
            aml_topic.dds_topic(),
            datawriter_qos,
            None)

        # Wait for a minimum time so the entity is created and discovered
        sleep(0.2)

    def write(self, data):
        """
        Write the data into the DDS DataWriter.

        :warning: data must be of kind of TopicDataType of the Writer.
        """
        self.writer_.write(data)

    def new_data(self):
        """Create a new Data of the data type that this Writer could write."""
        return self.aml_topic_.new_object()
