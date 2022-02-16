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
TODO comment
"""

from amlip_nodes.dds.entity.AmlTopic import AmlTopic
from amlip_nodes.dds.entity.AmlTopicDataType import AmlTopicDataType
from amlip_nodes.dds.entity.AmlParticipant import AmlParticipant
from amlip_nodes.exception.Exception import InconsistencyException

import fastdds


class TopicHandler():
    """
    TODO comment
    """

    def __init__(self):
        """
        TODO comment
        """
        self.topics_database_ = {}
        self.types_database_ = {}

    def register_data_type(
            self,
            topic_data_type_name,
            aml_participant: AmlParticipant,
            topic_data_type_pubsub_constructor,
            topic_data_type_constructor) -> AmlTopic:
        """TODO comment."""

        topic_data_type = None

        # Check it type exist and if not register it
        if not self.topic_data_type_exist(topic_data_type_name):
            # Register type in participant
            topic_data_type = AmlTopicDataType(
                aml_participant=aml_participant,
                topic_data_type_name=topic_data_type_name,
                topic_data_type_pubsub_constructor=topic_data_type_pubsub_constructor,
                topic_data_type_constructor=topic_data_type_constructor)

            self._add_data_type(topic_data_type)

            # print(f'Registering type {topic_data_type_name} in Participant '
            #       f'{aml_participant.name()}.')

            return topic_data_type

        else:
            # Get it from database
            return self.types_database_[topic_data_type_name]

    def register_topic(
            self,
            topic_name,
            topic_data_type_name,
            aml_participant: AmlParticipant,
            topic_qos=fastdds.TopicQos()) -> AmlTopic:
        """TODO comment."""

        # Check if it does already exist
        if self.topic_exist(topic_name):
            # If it exist, return it
            return self.topics_database_[topic_name]

        # Get type. If it is not registered fail
        topic_data_type = self.get_topic_data_type(topic_data_type_name)

        # Create Topic
        topic = AmlTopic(
            topic_name=topic_name,
            alm_participant=aml_participant,
            aml_topic_data_type=topic_data_type)

        self._add_topic(topic)

        # print(f'Registering topic {topic_name} in Participant '
        #       f'{aml_participant.name()}.')

        return topic

    def topic_exist(
            self,
            topic_name):
        """TODO comment."""
        return topic_name in self.topics_database_.keys()

    def topic_data_type_exist(
            self,
            topic_data_type_name):
        """TODO comment."""
        return topic_data_type_name in self.types_database_.keys()

    def get_topic(
            self,
            topic_name):
        """TODO comment."""
        if self.topic_exist(topic_name):
            return self.topics_database_[topic_name]
        else:
            raise InconsistencyException(
                f'Topic {topic_name} is not registered.')

    def get_topic_data_type(
            self,
            topic_data_type_name):
        """TODO comment."""
        if self.topic_data_type_exist(topic_data_type_name):
            return self.types_database_[topic_data_type_name]
        else:
            raise InconsistencyException(
                f'Topic {topic_data_type_name} is not registered.')

    def _add_data_type(
            self,
            aml_data_type: AmlTopicDataType):
        self.types_database_[aml_data_type.name()] = aml_data_type

    def _add_topic(
            self,
            aml_topic: AmlTopicDataType):
        self.topics_database_[aml_topic.name()] = aml_topic
