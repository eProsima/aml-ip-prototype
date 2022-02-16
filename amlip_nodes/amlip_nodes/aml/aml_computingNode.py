from pathlib import Path
import pickle
import time

from aml_config import *
from aml_types import *


class ComputingNode:
    def __init__(self):
        self.results_folder = Path(RESULTS_FOLDER)
        self.tasks_folder = Path(TASKS_FOLDER)

        self.model_info: ModelInfo
        self.atomization: list[Atom] = []
        self.duples: list[Duples] = []

    def start(self):
        while True:
            hasFile = False
            fileName = None
            while not fileName:
                for f in self.tasks_folder.iterdir():
                    if f.suffix == ".amljb":
                        fileName = f
                        break

                if not fileName:
                    time.sleep(1)
                    continue

                try:
                    temp_fileName = fileName.with_suffix(".j2temp")
                    fileName.rename(temp_fileName)
                    if temp_fileName.exists():
                        hasFile = True
                except FileNotFoundError:
                    continue

            if hasFile:
                self.loadJobOnFile(temp_fileName)

                self.calculation()

                result_fileName = self.results_folder / fileName.name

                print(f"<Saving {result_fileName.stem}>")
                self.saveAtomizationOnFile(result_fileName)
                print(f"<Removing {temp_fileName.stem}>")
                temp_fileName.unlink()


    def loadJobOnFile(self, file_name: Path):
        print(f"Loading job file {file_name}")
        with open(file_name, "rb") as inputfile:
            self.model_info = pickle.load(inputfile)
            atomization_len = pickle.load(inputfile)
            self.atomization = []
            for _ in range(atomization_len):
                at = pickle.load(inputfile)
                self.atomization.append(at)
            duples_len = pickle.load(inputfile)
            self.duples = []
            for _ in range(duples_len):
                dp = pickle.load(inputfile)
                self.duples.append(dp)

    def saveAtomizationOnFile(self, file_name: Path):
        result_fileName = file_name.with_suffix(".aml")
        temp_fileName = file_name.with_suffix(".atemp")
        with open(temp_fileName, "wb") as output:
            pickle.dump(self.model_info, output, pickle.HIGHEST_PROTOCOL)
            pickle.dump(len(self.atomization), output, pickle.HIGHEST_PROTOCOL)
            for at in self.atomization:
                pickle.dump(at, output, pickle.HIGHEST_PROTOCOL)
        temp_fileName.rename(result_fileName)

    def calculation(self):
        print("Performing some slow calculation", end="",flush=True)
        for _ in range(10):
            time.sleep(0.2)
            print(".", end="",flush=True)
        print()
