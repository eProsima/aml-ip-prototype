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

from enum import Enum


class EndpointKind(Enum):
    """Endpoint kind."""

    PUBSUB_WRITER = 0   # PubSub Writer ~ DDS DataWriter
    PUBSUB_READER = 1   # PubSub Reader ~ DDS DataReader
    MS_SERVER = 2       # MultiService Server
    MS_CLIENT = 3       # MultiService Client


class TopicKind(Enum):
    """TODO comment."""

    PUBSUB = 0                              # PubSub topic kind
    MULTISERVICE_REQUEST_AVAILABILITY = 1   # MultiService request availability kind
    MULTISERVICE_REPLY_AVAILABLE = 2        # MultiService reply available kind
    MULTISERVICE_TASK_TARGET = 3            # MultiService task target kind
    MULTISERVICE_TASK = 4                   # MultiService task kind
    MULTISERVICE_SOLUTION = 5               # MultiService solution kind
