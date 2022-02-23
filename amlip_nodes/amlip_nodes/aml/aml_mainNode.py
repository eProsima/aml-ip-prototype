"""AML Main Node Implementation."""

import random
import time
from threading import Condition, Thread

from amlip_nodes.aml.aml_config import RESULTS_FOLDER, checkFolders
from amlip_nodes.aml.aml_types import Atom, Duple, Job, JobSolution, ModelInfo
from amlip_nodes.dds.node.AmlDdsMainNode import AmlDdsMainNode
from amlip_nodes.log.log import logger


class MainNode:
    """
    This is a Aml Node that create jobs and send them to Computing Nodes.

    It has an AML DDS Main Node that implement the communication between nodes.

    This is a Mock over the real AML Main Node, and it create random new jobs
    at arbitrary times. The data generated is not valid AML data.
    """

    def __init__(
            self,
            name,
            store_in_file=True):
        """Create a default MainNode."""
        # Internal variables
        self.name_ = name
        self.store_in_file_ = store_in_file

        # DDS variables
        self.dds_main_node_ = AmlDdsMainNode(name)
        self.dds_main_node_.init()

        # Stop variables
        self.stop_ = False
        self.cv_stop_ = Condition()

        # Jobs
        self.job_number_ = 0
        self.pending_jobs_: list[Job] = []
        self.solved_jobs_: list[(Job, JobSolution)] = []
        self.timeout_jobs_: list[Job] = []

        # Aml not used data
        self.model_info_ = ModelInfo()
        self.atoms_: list[Atom] = []
        self.duples_: list[Duple] = []

    def __del__(self):
        """TODO comment."""
        self.stop()

        if self.store_in_file_:
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
            logger.execution(f'AmlNode {self.name_} generating job (approximately {sleep_time} ms).')
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

            logger.execution(
                f'{self.name_} checking job results.')

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
                    self.solved_jobs_.append((pending_job, job_solution))

                elif self.dds_main_node_.is_job_timeout(pending_job):
                    # Job request has timeout
                    timeout_indexes.append(pending_job.index)
                    self.timeout_jobs_.append(pending_job)
                    del self.pending_jobs_[i]

                else:
                    # Job is still pending
                    not_solved_jobs_indexes.append(pending_job.index)

            # Log in stdout the actual status
            if len(not_solved_jobs_indexes) > 0:
                self.__print_not_yet_solved_job_indexes(not_solved_jobs_indexes)
            if len(timeout_indexes) > 0:
                self.__print_timeout_job_indexes(timeout_indexes)

            # Publish tasks discarded for timeout
            for task_id in timeout_indexes:
                self.dds_main_node_.task_timeout(task_id)

            # Sleep for period time
            time.sleep(period_time)

    def _save_atomization_in_file(
            self,
            file_name: str = ''):
        """TODO comment."""
        if file_name == '':
            # Check result folder exists
            checkFolders(RESULTS_FOLDER)
            file_name = f'{RESULTS_FOLDER}/result_atomization_{"".join(self.name_.split())}.aml'

        logger.debug(f'Storing jobs history from node {self.name_} in file {file_name}')

        # Open file and write down solution
        with open(f'{file_name}', 'w') as file:

            # Write down pending jobs
            if len(self.pending_jobs_) > 0:
                file.write('PENDING\n')
                for job in self.pending_jobs_:
                    file.write(f'{job}\n')

            # Write down timeout jobs
            if len(self.timeout_jobs_) > 0:
                file.write('\nTIMEOUT\n')
                for job in self.timeout_jobs_:
                    file.write(f'{job}\n')

            # Write down solutioned jobs
            if len(self.solved_jobs_) > 0:
                file.write('\nSOLUTION\n')
                for job, solution_job in self.solved_jobs_:
                    file.write(f'{job} : {solution_job}\n')

    # Variables to create random jobs
    subjects_ = ['he ', 'she ', 'jack ', 'tim ', 'lorenzo ']
    verbs_ = ['was ', 'is ', 'like ']
    nouns_ = ['playing ', 'reading ', 'talking ', 'dancing ', 'speaking ', 'bubbling ']

    def __random_job_generator() -> str:
        return random.choice(MainNode.subjects_) + \
            random.choice(MainNode.verbs_) + \
            random.choice(MainNode.nouns_)

    def __print_send_job(
            self,
            job: Job):
        logger.user(
            f'Main Node {self.name_} has created a new job:\n'
            f'  JOB: {job}'
            f'\n')

    def __print_solved_job(
            self,
            job: Job,
            solution: JobSolution):
        logger.user(
            f'Main Node {self.name_} has received the solution to job:\n'
            f'  JOB: {job}\n'
            f' with the result:\n'
            f'  SOLUTION: {solution}'
            f'\n')

    def __print_not_yet_solved_job_indexes(
            self,
            not_solved_jobs_indexes: list):
        logger.execution(
            f'Main Node {self.name_} has not yet solution for jobs: {not_solved_jobs_indexes}'
            f'\n')

    def __print_timeout_job_indexes(
            self,
            timeout_jobs_indexes: list):
        logger.warning(
            f'Main Node {self.name_} has discarded jobs {timeout_jobs_indexes} for timeout.'
            f'\n')
