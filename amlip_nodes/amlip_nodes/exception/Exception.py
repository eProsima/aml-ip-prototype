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
Exceptions for project.

All exceptions produced by objects and entities of this project must inherit from
the same AmlException class in order to make it easy to handle them.
"""


class AmlException(Exception):
    """Abstract Exception to inherit every other exception used in the project."""

    pass


class InconsistencyException(AmlException):
    """
    Exception when there are some unexpected behavior.

    e.g. Unexpected messages in multiservice.
    e.g. Enumeration value out from its Enumeration valid values.
    """

    pass


class NoDataException(AmlException):
    """Raises when trying to access a data collector without data."""

    pass


class StopException(AmlException):
    """Raises when a running entity has been stopped from another thread."""

    pass
