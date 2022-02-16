"""AML Main Node Implementation."""
import pickle
import random
import time
from pathlib import Path

from amlip_nodes.aml.aml_config import RESULTS_FOLDER
from amlip_nodes.aml.aml_types import Atom, Duple, Job, ModelInfo


class MainNode:
    """
    This is a Aml Node that create jobs and send them to Computing Nodes.

    It has an AML DDS Main Node that implement the communication between nodes.

    This is a Mock over the real AML Main Node, and it create random new jobs
    at arbitrary times. The data generated is not valid AML data.
    """

    def __init__(
            self,
            name):
        """Create a default MainNode."""
        self.name_ = name
        self.results_folder_ = Path(RESULTS_FOLDER)
        self.stop_ = False

        self.job_number_ = 0
        self.jobs_: list[Job] = []

        self.model_info_ = ModelInfo()
        self.atoms_: list[Atom] = []
        self.duples_: list[Duple] = []

        self.atomization = list[Job] = []

    def __del__(self):
        """"""


    def stop(self):
        """Set this Node as stopped."""
        self.stop_ = True

    def random_job_generator(
            self,
            seed=1234,
            time_range=(10, 5000)):
        """
        Request for random jobs.

        Create random jobs to send them to Computing nodes.
        The time between each node is also random between the range given

        :param seed: seed for random generator.
        :param time_range: range time elapsed in milliseconds to create a new job
        """
        while not self.stop_:
            # Create new Job
            new_job = Job(self.job_number_, MainNode.__random_job_generator())
            self.job_number_ += 1
            self.jobs_.append(new_job)

            # Sleep for a random time
            sleep_time = random.randint(time_range)
            print(f'AmlNode {self.name_} waiting for {sleep_time} ms.')
            time.sleep(sleep_time)

    def _create_new_random_job(self):
        """Create a new job."""
        new_job = Job(self.job_number_, MainNode.__random_job_generator())
        new_job.

    def _convert_job_data_type_to_dds(
            job: Job):
        """Convert a job data type to the type expected to send in DDS."""
        # Get job string
        result = str(job)
        # Transform string in octet array
        data = list(result)
        return data

    def save_atomization_in_file(self, file_name):
        with open(file_name.with_suffix(".aml"), "rb") as inputfile:
            self.model_info = pickle.load(inputfile)
            self.atomization = []
            atomization_len = pickle.load(inputfile)
            for _ in range(atomization_len):
                at = pickle.load(inputfile)
                self.atomization.append(at)

    jobs = [
        'If a Lone Star Tick bites you, you may become allergic to red meat.',
        'The national animal of Scotland is the unicorn.',
        'The couple in the painting "American Gothic" are actually father and daughter and not husband and wife.',
        'Red, green, yellow, and orange bell peppers are all the same type of pepper with their color difference being caused by being at different stages of ripeness.',
        'Most people have an above-average number of arms.',
        'The Federal Emergency Management Agency (FEMA) uses Waffle House Restaurants being open or closed as one way to determine the effect of a storm and the likely scale of assistance required for disaster recovery.',
        'German Chocolate Cake has nothing to do with Germany. It was named after English-American Samuel German who developed a formulation of dark baking chocolate that came to be used in the cake recipe.',
        'If sound could travel through space, the noise that the sun would be the equivalent to a train horn from 1 meter away.',
        'In space, you dont need welding materials to get two metals to fuse. They will do it on their own if you place them close enough together.',
        'The US has lost 6 nuclear weapons.',
    ]

    def __random_job_generator() -> str:
        return random.choice(MainNode.jobs)
