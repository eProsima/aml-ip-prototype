from pathlib import Path
import random

from aml_config import *
from aml_mainNode import MainNode
from aml_types import *


def checkFolders():
    for folder in TASKS_FOLDER, RESULTS_FOLDER:
        Path(folder).mkdir(parents=True, exist_ok=True)


def createMockFiles():
    (Path(RESULTS_FOLDER) / "a.aml").touch(exist_ok=True)
    (Path(RESULTS_FOLDER) / "b.aml").touch(exist_ok=True)
    (Path(RESULTS_FOLDER) / "c.jtemp").touch(exist_ok=True)


if __name__ == "__main__":
    random.seed(123)
    # createMockFiles()
    checkFolders()

    main_node = MainNode()

    # Mock data
    main_node.atoms = [Atom() for _ in range(10)]
    main_node.duples = [Duple() for _ in range(12)]
    for at in main_node.atoms:
        at.currData = Bitarray(random.choices(range(100), k=random.randint(1, 10)))
        at.prevData = Bitarray(random.choices(range(100), k=random.randint(1, 10)))
    for dp in main_node.duples:
        dp.first = Bitarray(random.choices(range(100), k=random.randint(1, 10)))
        dp.second = Bitarray(random.choices(range(100), k=random.randint(1, 10)))

    main_node.start()

