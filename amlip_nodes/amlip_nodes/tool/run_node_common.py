"""Run Aml Computing Node."""

from distutils.debug import DEBUG
import random
import signal
from threading import Condition, Thread

from amlip_nodes.log.log import logger, logging


CV_STOP_ = Condition()
STOP_ = False


def sigint_signal_handler(signum, frame):
    """SIGINT Signal handler."""
    logger.user(f'Signal handler called with signal {signum}.')

    global STOP_

    # Stop application
    CV_STOP_.acquire()
    STOP_ = True
    CV_STOP_.notify_all()
    CV_STOP_.release()


def run_node(node_name, node_constructor, args=None):

    logging.basicConfig(level=logging.NOTSET - 10)
    logger.setLevel(logging.USER)

    # TODO input args

    # Create "unique" name
    name = node_name + ' ' + str(random.randint(0, 999)).zfill(3)

    logger.user(f'Starting {name} execution.')

    # Activate signal handler
    signal.signal(signal.SIGINT, sigint_signal_handler)

    # Check result folder exist
    # checkFolders()

    # Create node and run
    main_node = node_constructor(name)

    # Run it in a separate thread
    node_thread = Thread(target=main_node.run)
    node_thread.start()

    # Wait to stop
    CV_STOP_.acquire()
    CV_STOP_.wait_for(
        predicate=lambda:
            STOP_)
    CV_STOP_.release()

    logger.user(f'Signal received, stopping node {name}. This could take few seconds.')

    # Stop node
    main_node.stop()

    # Wait for node to terminate
    node_thread.join()

    logger.user(f'Finishing {name} execution.')
