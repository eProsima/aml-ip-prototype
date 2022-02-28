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
Blackbox DDS Tests.

Work in progress.
TODO
"""


def test_communicate_job_simple():
    """
    Test commnication between one AML-MainNode and one AML-ComputationalNode.

    Create one AML-MainNode and one AML-ComputationalNode and create 10 random jobs to communicate
    both nodes.
    Check that the message receibed in the ComputationalNode is the sabe sent by the MainNode
    and check that the solution received is the solution generated.
    """

