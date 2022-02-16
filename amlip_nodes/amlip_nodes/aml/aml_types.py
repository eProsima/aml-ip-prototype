from dataclasses import dataclass


@dataclass(init=False)
class Job:
    index: int
    data: str

    def __str__(self):
        return str(self.index) + '/' + self.data


@dataclass(init=False)
class JobSolution:
    index: int
    data: str

    def __str__(self):
        return str(self.index) + '/' + self.data


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
