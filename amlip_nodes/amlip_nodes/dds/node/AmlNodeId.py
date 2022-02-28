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
"""AML Node Id implementation."""

import uuid


class AmlNodeId():
    """
    Common class to store a Node unique id.

    It uses uuid class to generate unique ids.
    """

    def get_new_id():
        """Get unique Aml Node Id."""
        return AmlNodeId(uuid.uuid4())

    def __init__(
            self,
            uuid_data=None):
        """Create a new unique random Aml Id."""
        # In case id is not provided, create a new one
        if uuid_data is None:
            uuid_data = AmlNodeId.get_new_id()
        self.uuid_data_ = uuid_data

    def id_str(self):
        """Return uuid in string format."""
        return str(self.uuid_data_)

    def __eq__(self, __o: object) -> bool:
        """Check if two ids are equal."""
        return self.id_str() == __o.id_str()

    def __str__(self):
        """Serialize id to string by retrieving uuid string."""
        return self.id_str()
