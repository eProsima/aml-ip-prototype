"""AML Computing Node Implementation."""

import random
import time
from pathlib import Path
from threading import Condition, Thread

from amlip_nodes.aml.aml_config import RESULTS_FOLDER
from amlip_nodes.aml.aml_types import Atom, Duple, Job, JobSolution, ModelInfo
from amlip_nodes.dds.node.AmlDdsComputingNode import AmlDdsComputingNode
from amlip_nodes.exception.Exception import StopException


class ComputingNode:
    """
    This is a Aml Node that wait for jobs from Main Nodes, calculate the solution and send them.

    It has an AML DDS Computing Node that implement the communication between nodes.

    This is a Mock over the real AML Computing Node, and the solution to the
    jobs is merely a human untestable routine of modifying a string to upper case.
    """

    def __init__(
            self,
            name):
        """Create a default MainNode."""
        # Internal variables
        self.name_ = name
        self.time_range_ms_ = (2000, 7000)

        # DDS variables
        self.dds_computing_node_ = AmlDdsComputingNode(
            name=name,
            job_process_callback=self._job_process_callback)
        self.dds_computing_node_.init()

        # Stop variables
        self.stop_ = False
        self.cv_stop_ = Condition()

    def __del__(self):
        """TODO comment."""
        self.stop()

    def stop(self):
        """Set this Node as stopped and notify threads."""
        self.cv_stop_.acquire()
        self.stop_ = True
        self.cv_stop_.notify_all()
        self.cv_stop_.release()

    def run(self):
        """TODO comment."""
        # Thread to generate jobs randomly
        job_calulator_thread = \
            Thread(target=self._calculate_job_solution_routine)
        job_calulator_thread.start()

        # Wait to stop
        self.cv_stop_.acquire()
        self.cv_stop_.wait_for(
            predicate=lambda:
                self.stop_)
        self.cv_stop_.release()

        # Wait for all threads
        job_calulator_thread.join()

    def _calculate_job_solution_routine(
            self,
            seed=4321,
            time_range_ms=(2000, 7000)):
        """
        Request for random jobs.

        Create random jobs to send them to Computing nodes.
        The time between each node is also random between the range given.

        Once stop is set, it will not stop until the time range has elapsed.

        :param seed: seed for random generator
        :param time_range: range time elapsed in milliseconds to create a new job
        """
        while not self.stop_:

            try:
                # Try to answer to a new job
                self.dds_computing_node_.process_job()

            except StopException:
                print(f'{self.name_} stopped while calculating a solution.')
                break

    def _job_process_callback(
            self,
            job: Job):
        """TODO comment."""
        # Sleep to simulate long calculation
        sleep_time = random.randint(self.time_range_ms_[0], self.time_range_ms_[1]) / 1000
        print(
            f'{self.name_} calculating result for job {job.index} (approximately {sleep_time} ms).')
        time.sleep(sleep_time)

        # Generate solution
        print(
            f'{self.name_} finish calculating job {job.index}.')

        return JobSolution(job.index, job.data.upper())
