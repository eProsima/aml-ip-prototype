"""AML Main Node Implementation."""

import random
import time
from pathlib import Path
from threading import Condition, Thread

from amlip_nodes.aml.aml_config import RESULTS_FOLDER
from amlip_nodes.aml.aml_types import Atom, Duple, Job, JobSolution, ModelInfo
from amlip_nodes.dds.node.AmlDdsMainNode import AmlDdsMainNode


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
        # Internal variables
        self.name_ = name
        self.results_folder_ = Path(RESULTS_FOLDER)

        # DDS variables
        self.dds_main_node_ = AmlDdsMainNode(name)
        self.dds_main_node_.init()

        # Stop variables
        self.stop_ = False
        self.cv_stop_ = Condition()

        # Jobs
        self.job_number_ = 0
        self.pending_jobs_: list[Job] = []
        self.solved_jobs_: list[JobSolution] = []

        # Aml not used data
        self.model_info_ = ModelInfo()
        self.atoms_: list[Atom] = []
        self.duples_: list[Duple] = []

    def __del__(self):
        """TODO comment."""
        self.stop()
        self._save_atomization_in_file()

    def stop(self):
        """
        Stop this node.

        Stop its internal DDS entities.
        Set variable stop as true.
        Awake threads waiting for stop.
        """
        self.dds_main_node_.stop()

        self.cv_stop_.acquire()
        self.stop_ = True
        self.cv_stop_.notify_all()
        self.cv_stop_.release()

    def run(self):
        """TODO comment."""
        # Thread to generate jobs randomly
        random_job_generator_thread = \
            Thread(target=self.random_job_generator_routine_)
        random_job_generator_thread.start()

        # Thread to periodically check jobs pending and answered
        job_checker_thread = \
            Thread(target=self.check_jobs_solutions_routine_)
        job_checker_thread.start()

        # Wait to stop
        self.cv_stop_.acquire()
        self.cv_stop_.wait_for(
            predicate=lambda:
                self.stop_)
        self.cv_stop_.release()

        # Wait for all threads
        random_job_generator_thread.join()
        job_checker_thread.join()

    def random_job_generator_routine_(
            self,
            seed=1234,
            time_range_ms=(1000, 5000)):
        """
        Request for random jobs.

        Create random jobs to send them to Computing nodes.
        The time between each node is also random between the range given.

        Once stop is set, it will not stop until the time range has elapsed.

        :param seed: seed for random generator
        :param time_range: range time elapsed in milliseconds to create a new job
        """
        while not self.stop_:
            # Create new Job
            new_job = Job(self.job_number_, MainNode.__random_job_generator())
            self.job_number_ += 1
            self.pending_jobs_.append(new_job)

            # Send job
            self.dds_main_node_.send_job_async(new_job)

            # Print new job sent
            self.__print_send_job(new_job)

            # Sleep for a random time
            sleep_time = random.randint(time_range_ms[0], time_range_ms[1]) / 1000
            print(f'AmlNode {self.name_} generating job (approximately {sleep_time} ms).')
            time.sleep(sleep_time)

    def check_jobs_solutions_routine_(
            self,
            period_time=5):
        """
        Check if any pending job has been answered.

        Once stop is set, it will not stop until the time range has elapsed.

        :param period_time: time to wait between checks

        :todo: in the future it should not wait for a time but with a dds method in DdsAmlMainNode
        """

        while not self.stop_:

            print(f'AmlNode {self.name_} checking job results.')

            not_solved_jobs_indexes = []
            timeout_indexes = []

            # For each pending job, check if solution has arrived
            # Iterate backwards the list so it can erase elements at the same time as iterating
            for i in range(len(self.pending_jobs_) - 1, -1, -1):
                pending_job = self.pending_jobs_[i]

                # Check it in Dds Node by checking the index
                if self.dds_main_node_.is_job_answered(pending_job):
                    # Job Solved!
                    job_solution = self.dds_main_node_.get_job_response(pending_job)

                    # Print result
                    self.__print_solved_job(pending_job, job_solution)

                    # Removing pending job
                    del self.pending_jobs_[i]

                    # Store it as solved job
                    self.solved_jobs_.append(job_solution)

                elif self.dds_main_node_.is_job_timeout(pending_job):
                    # Job request has timeout
                    timeout_indexes.append(pending_job.index)
                    del self.pending_jobs_[i]

                else:
                    # Job is still pending
                    not_solved_jobs_indexes.append(pending_job.index)

            if len(not_solved_jobs_indexes) > 0:
                self.__print_not_yet_solved_job_indexes(not_solved_jobs_indexes)
            if len(timeout_indexes) > 0:
                self.__print_timeout_job_indexes(timeout_indexes)

            # Sleep for period time
            time.sleep(period_time)

    def _save_atomization_in_file(
            self,
            file_name: str = 'result_atomization.aml'):
        """TODO comment."""
        # TODO
        print(f'This is supposed to save the results in file {self.results_folder_}/{file_name}.')

    # Variables to create random jobs
    subjects_ = ['he ', 'she ', 'jack ', 'tim ']
    verbs_ = ['was ', 'is ']
    nouns_ = ['playing.', 'reading.', 'talking.', 'dancing.', 'speaking.']

    def __random_job_generator() -> str:
        return random.choice(MainNode.subjects_) + \
            random.choice(MainNode.verbs_) + \
            random.choice(MainNode.nouns_)

    def __print_send_job(
            self,
            job: Job):
        print(
            f'\n'
            f'Main Node {self.name_} has created a new job:\n'
            f'  JOB: {job}')

    def __print_solved_job(
            self,
            job: Job,
            solution: JobSolution):
        print(
            f'\n'
            f'Main Node {self.name_} has received the solution to job:\n'
            f'  JOB: {job}\n'
            f' with the result:\n'
            f'  SOLUTION: {solution}')

    def __print_not_yet_solved_job_indexes(
            self,
            not_solved_jobs_indexes: list):
        print(
            f'\n'
            f'Main Node {self.name_} has not yet solution for jobs: {not_solved_jobs_indexes}')

    def __print_timeout_job_indexes(
            self,
            timeout_jobs_indexes: list):
        print(
            f'\n'
            f'Main Node {self.name_} has discarded jobs {timeout_jobs_indexes} for timeout.')
