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

from amlip_nodes.dds.network.aml_node_types import TopicKind
from amlip_nodes.exception.Exception import InconsistencyException

# Status topic
STATUS_TOPIC_NAME = 'status'

# Job topic
JOB_SERVICE_NAME = 'job'

# Inference topic
INFERENCE_SERVICE_NAME = 'inference'


def aml_topic_mangling(
        topic_name: str,
        topic_kind: TopicKind) -> str:
    """
    Return name of topic after mangling.

    The mangling depends on the topic kind:
    - PubSub :
        "aml"
        "/" + topic name
    - MultiService:
        "aml/ms"
        "/" + multiservice step topic name
        "/" + topic name
    """
    if topic_kind == TopicKind.PUBSUB:
        return 'aml/' + topic_name
    elif topic_kind == TopicKind.MULTISERVICE_REQUEST_AVAILABILITY:
        return 'aml/ms/request_availability/' + topic_name
    elif topic_kind == TopicKind.MULTISERVICE_REPLY_AVAILABLE:
        return 'aml/ms/reply_available/' + topic_name
    elif topic_kind == TopicKind.MULTISERVICE_TASK_TARGET:
        return 'aml/ms/task_target/' + topic_name
    elif topic_kind == TopicKind.MULTISERVICE_TASK:
        return 'aml/ms/task/' + topic_name
    elif topic_kind == TopicKind.MULTISERVICE_SOLUTION:
        return 'aml/ms/solution/' + topic_name
    else:
        raise InconsistencyException(f'TopicKind {topic_kind} does not exist.')


def aml_topic_data_type_mangling(
        topic_data_type_name: str,
        topic_kind: TopicKind) -> str:
    """
    Return name of topic data type after mangling.

    The mangling depends on the topic kind:
    - PubSub :
        "aml"
        "::" + topic data type name
    - MultiService:
        "aml::ms"
        "::" + multiservice topic data type name
        "::" + topic data type name
    """
    if topic_kind == TopicKind.PUBSUB:
        return 'aml::' + topic_data_type_name
    elif topic_kind == TopicKind.MULTISERVICE_REQUEST_AVAILABILITY:
        return 'aml::ms::request'
    elif topic_kind == TopicKind.MULTISERVICE_REPLY_AVAILABLE:
        return 'aml::ms::task_reference'
    elif topic_kind == TopicKind.MULTISERVICE_TASK_TARGET:
        return 'aml::ms::task_reference'
    elif topic_kind == TopicKind.MULTISERVICE_TASK:
        return 'aml::ms::task::' + topic_data_type_name
    elif topic_kind == TopicKind.MULTISERVICE_SOLUTION:
        return 'aml::ms::solution::' + topic_data_type_name
    else:
        raise InconsistencyException(f'TopicKind {topic_kind} does not exist.')
