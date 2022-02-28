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

from amlip_nodes.aml.aml_types import Job, JobSolution


indexes = [0, 12, 12540]
datas = ['data.', 'data of job2 with chars %&$#,._+!;?<>', 'badabadum']


def test_amltypes_create_job():
    """TODO comment."""
    for index in indexes:
        for data in datas:
            job_data = Job(index, data)
            assert str(job_data) == '{' + f'{index}: <{data}>' + '}'


def test_amltypes_transform_job_to_ddsjob():
    """TODO comment."""
    for index in indexes:
        for data in datas:
            job_data = Job(index, data)
            ddsjob = job_data.to_dds_data_type()
            assert ddsjob


def test_amltypes_transform_job_to_ddsjob_to_job():
    """TODO comment."""
    for index in indexes:
        for data in datas:
            job_data = Job(index, data)
            ddsjob = job_data.to_dds_data_type()
            job_data_retrieve = Job.from_dds_data_type(ddsjob)
            assert job_data_retrieve.index == job_data.index
            assert job_data_retrieve.data == job_data.data


def test_amltypes_create_jobsolution():
    """TODO comment."""
    for index in indexes:
        for data in datas:
            jobsolution_data = JobSolution(index, data)
            assert str(jobsolution_data) == '{' + f'{index}: <{data}>' + '}'


def test_amltypes_transform_jobsolution_to_ddsjobsolution():
    """TODO comment."""
    for index in indexes:
        for data in datas:
            jobsolution_data = JobSolution(index, data)
            ddsjobsolution = jobsolution_data.to_dds_data_type()
            assert ddsjobsolution


def test_amltypes_transform_jobsolution_to_ddsjobsolution_to_jobsolution():
    """TODO comment."""
    for index in indexes:
        for data in datas:
            jobsolution_data = JobSolution(index, data)
            ddsjobsolution = jobsolution_data.to_dds_data_type()
            jobsolution_data_retrieve = JobSolution.from_dds_data_type(ddsjobsolution)
            assert jobsolution_data_retrieve.index == jobsolution_data.index
            assert jobsolution_data_retrieve.data == jobsolution_data.data
