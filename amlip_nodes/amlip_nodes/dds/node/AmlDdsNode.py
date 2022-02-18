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
AML Node Participant.

Interface of an AML DDS Participant.
It gives methods to easily create AML entities inside this Participant.

It implements certain common routines for all AML Participants, as
 - status report: Writer in topic status that publish the current status.
 - pubsub creation
 - multiservice creation
"""

import amlip_nodes.dds.network.aml_topic_names as topic_names
from amlip_nodes.dds.entity.AmlMultiserviceClient import AmlMultiserviceClient
from amlip_nodes.dds.entity.AmlMultiserviceServer import AmlMultiserviceServer
from amlip_nodes.dds.entity.AmlParticipant import AmlParticipant
from amlip_nodes.dds.entity.AmlReader import AmlReader
from amlip_nodes.dds.entity.AmlTopic import AmlTopic
from amlip_nodes.dds.entity.AmlTopicDataType import AmlTopicDataType
from amlip_nodes.dds.entity.AmlWriter import AmlWriter
from amlip_nodes.dds.network.aml_node_types import TopicKind
from amlip_nodes.dds.node.AmlNodeId import AmlNodeId
from amlip_nodes.dds.node.TopicHandler import TopicHandler

import multiservice

import status


class AmlDdsNode():
    """
    AML Node Participant interface.

    This class handles a DDS Participant inside an AML Participant object.
    It also handles every AML and DDS subentity.
    """

    def __init__(
            self,
            name,
            domain=0):
        """Construct AmlNode and instantiate every internal variable."""
        # Participant Id
        self.id_ = AmlNodeId()

        # Internal variables
        self.name_ = name
        self.domain_ = domain
        self.enabled_ = False
        # DDS variables
        self.participant_ = None
        self.topic_handler_ = TopicHandler()
        self.status_writer_ = None

        print(f'Created AML Participant {name} with id {self.id_}.')

    def init(self):
        """
        Construct MainNodeParticipant object.

        Construct MainNodeParticipant with name and domain given by arguments.
        Create DataWriter for Status topic.
        """
        # Construct Participant
        self.participant_ = AmlParticipant(
            name=self.name_,
            domain=self.domain_)

        # Create Writer
        self.status_writer_ = self._create_writer(
            topic_name=topic_names.STATUS_TOPIC_NAME,
            topic_data_type_pubsub_constructor=status.StatusPubSubType,
            topic_data_type_constructor=status.Status)

        # publish state
        self._publish_status_data()

        print(f'AML Participant {self.name_} created.')

    def _publish_status_data(self, alive=True):
        """Write in Status writer the actual status of the entity."""
        # Get data type object
        data = self.status_writer_.new_data()
        # Fill id
        data.id(self.aml_id().id_str())
        # Fill name
        data.name(self.name_)
        # Fill status
        if alive:
            data.status(status.RUNNING)
        else:
            data.status(status.DISABLED)
        # Fill node kind getting it from override method
        data.node_kind(self._node_kind())

        # Send data
        self.status_writer_.write(data)

    def _node_kind(self):
        """
        Kind of the node.

        This method should be overload in subentities so each of them
        has a specific kind.
        """
        return status.UNDETERMINED

    def aml_id(self):
        """Return id object of this node."""
        return self.id_

    def _register_data_type(
            self,
            topic_data_type_name: str,
            topic_data_type_pubsub_constructor,
            topic_data_type_constructor) -> AmlTopicDataType:
        """
        Register a new Data Type in DDS Participant.

        It uses the internal TopicHandler to handle the registration.

        If the type was already registered, it gets the one already registered.

        :return: AmlTopicDataType registered.
        """
        return self.topic_handler_.register_data_type(
            topic_data_type_name=topic_data_type_name,
            aml_participant=self.participant_,
            topic_data_type_pubsub_constructor=topic_data_type_pubsub_constructor,
            topic_data_type_constructor=topic_data_type_constructor)

    def _register_topic(
            self,
            topic_name: str,
            topic_data_type_name: str) -> AmlTopic:
        """
        Register a new Data Type in DDS Participant.

        It uses the internal TopicHandler to handle the registration.

        If the type was already registered, it gets the one already registered.

        :return: AmlTopic registered.
        """
        return self.topic_handler_.register_topic(
            topic_name=topic_name,
            topic_data_type_name=topic_data_type_name,
            aml_participant=self.participant_)

    def _create_writer(
            self,
            topic_name: str,
            topic_data_type_name=None,
            topic_data_type_pubsub_constructor=None,
            topic_data_type_constructor=None) -> AmlWriter:
        return self.__create_endpoint(
            topic_name=topic_name,
            topic_data_type_name=topic_data_type_name,
            topic_data_type_pubsub_constructor=topic_data_type_pubsub_constructor,
            topic_data_type_constructor=topic_data_type_constructor,
            writer=True)

    def _create_reader(
            self,
            topic_name: str,
            topic_data_type_name=None,
            topic_data_type_pubsub_constructor=None,
            topic_data_type_constructor=None) -> AmlReader:
        return self.__create_endpoint(
            topic_name=topic_name,
            topic_data_type_name=topic_data_type_name,
            topic_data_type_pubsub_constructor=topic_data_type_pubsub_constructor,
            topic_data_type_constructor=topic_data_type_constructor,
            writer=False)

    def __create_endpoint(
            self,
            topic_name: str,
            topic_data_type_name=None,
            topic_data_type_pubsub_constructor=None,
            topic_data_type_constructor=None,
            writer=True):
        """
        Create a DataWriter.

        Create a Publisher with one DataWriter in specific topic.
        """
        """ Get Topic Data Type """
        if topic_data_type_name is None:
            topic_data_type_name = topic_name

        # Mangling
        topic_data_type_name_mangled = \
            topic_names.aml_topic_data_type_mangling(
                topic_data_type_name=topic_name,
                topic_kind=TopicKind.PUBSUB)

        # Look for it or create it in case constructor is given
        # If constructor is not given, it should already exist
        if topic_data_type_constructor is not None:
            self._register_data_type(
                topic_data_type_name_mangled,
                topic_data_type_pubsub_constructor,
                topic_data_type_constructor)

        """ Get Topic """
        # Mangling
        topic_name_mangled = \
            topic_names.aml_topic_mangling(
                topic_name=topic_name,
                topic_kind=TopicKind.PUBSUB)

        # Get Topic or create it
        aml_topic = \
            self._register_topic(
                topic_name_mangled,
                topic_data_type_name_mangled)

        """ Create Entity """
        if writer:
            # Create Writer
            print(f'Creating Writer in Participant {self.name_} '
                  f'in topic {topic_name}.')

            return AmlWriter(
                aml_topic=aml_topic,
                aml_participant=self.participant_)

        else:
            # Create Reader
            print(f'Creating Reader in Participant {self.name_} '
                  f'in topic {topic_name}.')
            return AmlReader(
                aml_topic=aml_topic,
                aml_participant=self.participant_)

    def _create_multiservice_client(
            self,
            service_name: str,
            topic_data_type_data_pubsub_constructor,
            topic_data_type_data_constructor,
            topic_data_type_solution_pubsub_constructor,
            topic_data_type_solution_constructor) -> AmlMultiserviceClient:
        return self.__create_multiservice(
            service_name=service_name,
            topic_data_type_data_pubsub_constructor=topic_data_type_data_pubsub_constructor,
            topic_data_type_data_constructor=topic_data_type_data_constructor,
            topic_data_type_solution_pubsub_constructor=topic_data_type_solution_pubsub_constructor,
            topic_data_type_solution_constructor=topic_data_type_solution_constructor,
            server=False)

    def _create_multiservice_server(
            self,
            service_name: str,
            topic_data_type_data_pubsub_constructor,
            topic_data_type_data_constructor,
            topic_data_type_solution_pubsub_constructor,
            topic_data_type_solution_constructor,
            callback=None) -> AmlMultiserviceServer:
        return self.__create_multiservice(
            service_name=service_name,
            topic_data_type_data_pubsub_constructor=topic_data_type_data_pubsub_constructor,
            topic_data_type_data_constructor=topic_data_type_data_constructor,
            topic_data_type_solution_pubsub_constructor=topic_data_type_solution_pubsub_constructor,
            topic_data_type_solution_constructor=topic_data_type_solution_constructor,
            callback=callback,
            server=True)

    def __create_multiservice(
            self,
            service_name: str,
            topic_data_type_data_pubsub_constructor,
            topic_data_type_data_constructor,
            topic_data_type_solution_pubsub_constructor,
            topic_data_type_solution_constructor,
            callback=None,
            server=True):
        """
        Create the entities needed to have a Service Server.

        Check if topics and data types are already created and create them.
        Create datawriter in reply_available and reply_solution
        Create datareader in request and request_data
        """
        """ Create every Data Type """
        # Mangling
        request_availability_data_type_name = topic_names.aml_topic_data_type_mangling(
            service_name, TopicKind.MULTISERVICE_REQUEST_AVAILABILITY)
        task_reference_data_type_name = topic_names.aml_topic_data_type_mangling(
            service_name, TopicKind.MULTISERVICE_REPLY_AVAILABLE)
        task_type_name = topic_names.aml_topic_data_type_mangling(
            service_name, TopicKind.MULTISERVICE_TASK)
        solution_data_type_name = topic_names.aml_topic_data_type_mangling(
            service_name, TopicKind.MULTISERVICE_SOLUTION)

        # Request Availability Data Type
        self._register_data_type(
            topic_data_type_name=request_availability_data_type_name,
            topic_data_type_pubsub_constructor=multiservice.Multiservice_RequestAvailabilityPubSubType,
            topic_data_type_constructor=multiservice.Multiservice_RequestAvailability)

        # Reply Available Data Type
        self._register_data_type(
            topic_data_type_name=task_reference_data_type_name,
            topic_data_type_pubsub_constructor=multiservice.Multiservice_TaskReferencePubSubType,
            topic_data_type_constructor=multiservice.Multiservice_TaskReference)

        # Task Data Type
        self._register_data_type(
            topic_data_type_name=task_type_name,
            topic_data_type_pubsub_constructor=topic_data_type_data_pubsub_constructor,
            topic_data_type_constructor=topic_data_type_data_constructor)

        # Solution Type
        self._register_data_type(
            topic_data_type_name=solution_data_type_name,
            topic_data_type_pubsub_constructor=topic_data_type_solution_pubsub_constructor,
            topic_data_type_constructor=topic_data_type_solution_constructor)

        """ Create every topic"""
        # Mangling
        request_availability_topic_name = topic_names.aml_topic_mangling(
            service_name, TopicKind.MULTISERVICE_REQUEST_AVAILABILITY)
        reply_available_topic_name = topic_names.aml_topic_mangling(
            service_name, TopicKind.MULTISERVICE_REPLY_AVAILABLE)
        task_target_topic_name = topic_names.aml_topic_mangling(
            service_name, TopicKind.MULTISERVICE_TASK_TARGET)
        task_topic_name = topic_names.aml_topic_mangling(
            service_name, TopicKind.MULTISERVICE_TASK)
        solution_topic_name = topic_names.aml_topic_mangling(
            service_name, TopicKind.MULTISERVICE_SOLUTION)

        # aml_request_availability_topic
        aml_request_availability_topic = \
            self._register_topic(
                topic_name=request_availability_topic_name,
                topic_data_type_name=request_availability_data_type_name)

        # aml_reply_available_topic
        aml_reply_available_topic = \
            self._register_topic(
                topic_name=reply_available_topic_name,
                topic_data_type_name=task_reference_data_type_name)

        # aml_task_target_topic
        aml_task_target_topic = \
            self._register_topic(
                topic_name=task_target_topic_name,
                topic_data_type_name=task_reference_data_type_name)

        # aml_task_topic
        aml_task_topic = \
            self._register_topic(
                topic_name=task_topic_name,
                topic_data_type_name=task_type_name)

        # aml_solution_topic
        aml_solution_topic = \
            self._register_topic(
                topic_name=solution_topic_name,
                topic_data_type_name=solution_data_type_name)

        """ Create entity """
        if server:
            # Create Server
            print(f'Creating Server in Participant {self.name_} '
                  f'in service {service_name}.')

            return AmlMultiserviceServer(
                node_id=self.id_,
                service_name=service_name,
                aml_participant=self.participant_,
                aml_reader_request_availability_topic=aml_request_availability_topic,
                aml_writer_reply_available_topic=aml_reply_available_topic,
                aml_reader_task_target_topic=aml_task_target_topic,
                aml_reader_task_topic=aml_task_topic,
                aml_writer_solution_topic=aml_solution_topic,
                callback=callback)

        else:
            # Create Client
            print(f'Creating Client in Participant {self.name_} '
                  f'in service {service_name}.')

            return AmlMultiserviceClient(
                node_id=self.id_,
                service_name=service_name,
                aml_participant=self.participant_,
                aml_writer_request_availability_topic=aml_request_availability_topic,
                aml_reader_reply_available_topic=aml_reply_available_topic,
                aml_writer_task_target_topic=aml_task_target_topic,
                aml_writer_task_topic=aml_task_topic,
                aml_reader_solution_topic=aml_solution_topic)

    def stop(self):
        """
        Stop the node.

        Overload it in subclasses.
        """
        pass

    def __del__(self):
        """
        Destroy Entity.

        So far, every entity is destroyed by itself.
        """
        self._publish_status_data(alive=False)
        print(f'Destroying Node {self.name_}.')
