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
"""Implement a data structure TopicDataType that wraps the DDS TopicDataType entity."""


from amlip_nodes.dds.entity.AmlParticipant import AmlParticipant
from amlip_nodes.log.log import logger

import fastdds


class AmlTopicDataType():
    """
    AmlTopicDataType entity.

    It could be related to none, one or several topics.
    """

    def __init__(
            self,
            aml_participant: AmlParticipant,
            topic_data_type_name,
            topic_data_type_pubsub_constructor,
            topic_data_type_constructor):
        """
        Create new AmlTopicDataType.

        Registers the new DatType into the Participant.

        :param topic_data_type_pubsub_constructor: constructor function of the class PubSubType
            of this data type.
        :param topic_data_type_constructor: constructor function of the class
            of this data type.
        """
        logger.construct(f'Creating AmlTopicDataType {topic_data_type_name}.')

        self.name_ = topic_data_type_name
        # self.topic_data_type_pubsub_constructor_ = topic_data_type_pubsub_constructor
        self.topic_data_type_constructor_ = topic_data_type_constructor

        # Register data
        topic_data_type_pubsub_object = topic_data_type_pubsub_constructor()
        topic_data_type_pubsub_object.setName(self.name())
        type_support = fastdds.TypeSupport(topic_data_type_pubsub_object)
        aml_participant.get_dds_participant().register_type(type_support)

    def name(self):
        """Return name of this data type."""
        return self.name_

    def new_object(self):
        """Return a new empty object of the class this DataType refers."""
        return self.topic_data_type_constructor_()
