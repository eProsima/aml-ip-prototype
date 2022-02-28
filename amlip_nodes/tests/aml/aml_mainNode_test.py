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

from threading import Thread
from time import sleep

from amlip_nodes.aml.aml_mainNode import MainNode


def test_main_node_creation():
    """Create a MainNode."""
    node = MainNode(
        name='MainNode',
        store_in_file=False)
    node.stop()


def test_main_node_run():
    """Create a MainNode."""
    node = MainNode(
        name='MainNode',
        store_in_file=False)

    node_thread = Thread(target=node.run)
    node_thread.start()

    sleep(1)

    node.stop()

    node_thread.join()
