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
"""Entity that has a DDS Participant and wrap its methods."""

import fastdds

from amlip_nodes.log.log import logger


class AmlParticipant():
    """AmlParticipant entity."""

    def __init__(
            self,
            name,
            domain=0,
            participant_qos=fastdds.DomainParticipantQos()):
        """
        Create a AmlParticipant entity.

        Create every internal variable for class I_AmlNodeParticipant.
        """
        logger.construct(f'Creating AmlParticipant {name}.')

        # Participant internal values
        self.name_ = name

        # Get Participant QoS
        factory = fastdds.DomainParticipantFactory.get_instance()
        participant_qos.name = name  # TODO Check it is working

        # Construct participant
        self.participant_ = factory.create_participant(
            domain,
            participant_qos)

    def get_dds_participant(self):
        """Return reference to participant."""
        return self.participant_

    def name(self):
        """Return Participant name."""
        return self.name_

    def __del__(self):
        """Destroy Participant entities and destroy participant in factory."""
        logger.construct(f'Destroying AmlParticipant {self.name_}.')

        # Destroy alive entities
        self.participant_.delete_contained_entities()

        # Destroy participant
        fastdds.DomainParticipantFactory.get_instance().delete_participant(
            self.participant_)
