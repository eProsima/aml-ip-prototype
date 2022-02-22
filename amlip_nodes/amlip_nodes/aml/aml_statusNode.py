"""AML Status Node Implementation."""

from asyncio.log import logger
from threading import Condition, Thread

from amlip_nodes.aml.aml_config import RESULTS_FOLDER, checkFolders
from amlip_nodes.dds.node.AmlDdsStatusNode import AmlDdsStatusNode


class StatusNode:
    """
    TODO comment.
    """

    def __init__(
            self,
            name,
            store_in_file=True):
        """Create a default StatusNode."""
        # Internal variables
        self.name_ = name
        self.store_in_file_ = store_in_file

        # DDS variables
        self.dds_status_node_ = AmlDdsStatusNode(
            name=name)
        self.dds_status_node_.init()

        # Stop variables
        self.stop_ = False
        self.cv_stop_ = Condition()

    def __del__(self):
        """TODO comment."""
        self.stop()

        if self.store_in_file_:
            self._save_status_in_file()

    def run(self):
        """TODO comment."""
        # Thread to generate jobs randomly
        status_read_thread = \
            Thread(target=self._status_read_routine)
        status_read_thread.start()

        # Wait to stop
        self.cv_stop_.acquire()
        self.cv_stop_.wait_for(
            predicate=lambda:
                self.stop_)
        self.cv_stop_.release()

        # Wait for all threads
        status_read_thread.join()

    def stop(self):
        """
        Stop this node.

        Stop its internal DDS entities.
        Set variable stop as true.
        Awake threads waiting for stop.
        """
        self.dds_status_node_.stop()

        self.cv_stop_.acquire()
        self.stop_ = True
        self.cv_stop_.notify_all()
        self.cv_stop_.release()

    def _status_read_routine(self):
        """TODO comment."""
        self.dds_status_node_.process_status_async_routine()

    def _save_status_in_file(
        self,
        file_name: str = ''
    ):
        """Print in file the status history."""
        if file_name == '':
            checkFolders(RESULTS_FOLDER)
            file_name = f'{RESULTS_FOLDER}/status_history_{"".join(self.name_.split())}.st'

        logger.debug(f'Storing status from node {self.name_} in file {file_name}')

        with open(f'{file_name}', 'w') as file:
            for time, status in self.dds_status_node_.status_history_:
                file.write(f'TIME: {time}\n{AmlDdsStatusNode._str_status_data(status)}\n')
