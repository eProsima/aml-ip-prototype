"""AML Status Node Implementation."""

from threading import Condition, Thread

from amlip_nodes.dds.node.AmlDdsStatusNode import AmlDdsStatusNode


class StatusNode:
    """
    TODO comment.
    """

    def __init__(
            self,
            name):
        """Create a default StatusNode."""
        # Internal variables
        self.name_ = name

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
