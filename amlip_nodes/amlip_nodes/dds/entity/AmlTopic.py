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
"""Implement a data structure Topic that wraps the DDS Topic entity."""

from amlip_nodes.dds.entity.AmlParticipant import AmlParticipant
from amlip_nodes.dds.entity.AmlTopicDataType import AmlTopicDataType
from amlip_nodes.log.log import logger

import fastdds


class AmlTopic():
    """
    AmlTopic entity.

    It is directly related with a AML Topic Data Type.
    """

    def __init__(
            self,
            topic_name,
            alm_participant: AmlParticipant,
            aml_topic_data_type: AmlTopicDataType,
            topic_qos=fastdds.TopicQos()):
        """Create new AmlTopic by a Participant and a TopicDataType."""
        logger.construct(f'Creating AmlTopic {topic_name}.')

        self.topic_name_ = topic_name
        self.aml_topic_data_type_ = aml_topic_data_type

        # Create Topic
        self.dds_topic_ = alm_participant.get_dds_participant().create_topic(
            self.name(),
            self.aml_topic_data_type_.name(),
            topic_qos)

    def name(self):
        """Return name of the topic."""
        return self.topic_name_

    def type_name(self):
        """Return name of the topic data type."""
        return self.aml_topic_data_type_.name()

    def new_object(self):
        """Return a new empty object of the type of the data type for this topic."""
        return self.aml_topic_data_type_.new_object()

    def dds_topic(self):
        """Return DDS Topic object."""
        return self.dds_topic_
