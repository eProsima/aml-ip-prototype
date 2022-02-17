"""AML Types."""
from dataclasses import dataclass

import job


class Job:
    """TODO comment."""

    def __init__(
            self,
            index: int,
            data: str):
        """TODO comment."""
        self.index = index
        self.data = data

    def __str__(self):
        """Serialize a Job into a string."""
        return '{' + str(self.index) + ': <' + self.data + '>}'

    def to_dds_data_type(self):
        """Convert a job data type to the type expected to send in DDS."""
        dds_data = job.Job_Task_Data()
        # Get string of Job
        job_str = str(self)
        # Store this string in bytes
        dds_data.job().push_back(3)
        #dds_data.job(list(job_str))
        return dds_data


class JobSolution (Job):
    """TODO comment."""

    def __init__(
            self,
            index: int,
            data: str):
        """TODO comment."""
        super().__init__(index, data)

    def __str__(self):
        """Serialize a JobSolution into a string."""
        return str(self.index) + '/' + self.data

    def from_dds_data_type(dds_job):
        """Convert a job data type to the type expected to send in DDS."""
        # Convert solution to string
        result_str = ''
        for c in dds_job:
            result_str += ''
        result_str_split = result_str.split('/')

        # Get JobSolution data
        return JobSolution(int(result_str_split[0]), result_str_split[1])

class Bitarray(set):
    pass


# class Bitarray:
#     def __init__(self, values=None):
#         self.internal_value = ffi.new("void **")
#         internal_lib.add(self.internal_value, values)

#     def __del__(self):
#         internal_lib.delete(self.internal_value)


@dataclass(init=False)
class ModelInfo:
    index: int = 0
    params: dict


@dataclass(init=False)
class Atom:
    index: int = 0
    age: int = 0
    flag: bool = False
    currData: Bitarray
    prevData: Bitarray


@dataclass(init=False)
class Duple:
    index: int = 0
    age: int = 0
    flag: bool = False
    first: Bitarray
    second: Bitarray
