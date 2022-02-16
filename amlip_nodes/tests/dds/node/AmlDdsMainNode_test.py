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

from amlip_nodes.dds.node.AmlDdsMainNode import AmlDdsMainNode


def test_main_node_creation():
    """Create a AmlDdsMainNode."""
    node = AmlDdsMainNode(
        name='AmlDdsMainNode',
        inference_process_callback=None)
    node.init()
    node.stop()
