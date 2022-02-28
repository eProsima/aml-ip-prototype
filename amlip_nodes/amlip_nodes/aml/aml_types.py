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

    def to_dds_data_type(self) -> job.Job_Task_Data:
        """Convert a Job data type to the octet array expected to send in DDS."""
        # Result
        dds_data = job.Job_Task_Data()

        # Store this string in bytes
        for character in list(str(self)):
            dds_data.job().push_back(ord(character))
        return dds_data

    @staticmethod
    def from_dds_data_type(
            dds_job: job.Job_Task_Data) -> 'Job':
        """
        Convert a Solution Data octet array to a new JobSolution object.

        :note: using '' (str) to declare return value is done for forwarding declaration
        """
        # Convert job data to string byte by byte
        result_str = ''
        for byte_value in dds_job.job():
            result_str += chr(byte_value)
        result_str_split = result_str.split(':')

        # Get JobSolution data
        return Job(int(result_str_split[0][1:]), result_str_split[1][2:-2])


class JobSolution (Job):
    """TODO comment."""

    def to_dds_data_type(self) -> job.Job_Solution_Data:
        """Convert a JobSolution data type to the octet array expected to send in DDS."""
        # Result
        dds_data = job.Job_Solution_Data()

        # Store this string in bytes
        for character in list(str(self)):
            dds_data.solution().push_back(ord(character))
        return dds_data

    @staticmethod
    def from_dds_data_type(
            dds_job: job.Job_Solution_Data) -> 'JobSolution':
        """
        Convert a Solution Data octet array to a new JobSolution object.

        :note: using '' (str) to declare return value is done for forwarding declaration
        """
        # Convert solution data to string byte by byte
        result_str = ''
        for byte_value in dds_job.solution():
            result_str += chr(byte_value)
        result_str_split = result_str.split(':')

        # Get JobSolution data
        return JobSolution(int(result_str_split[0][1:]), result_str_split[1][2:-2])


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
