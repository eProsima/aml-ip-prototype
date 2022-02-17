"""Run Aml Computing Node."""

import random
import signal
from threading import Condition, Thread

from amlip_nodes.aml.aml_config import checkFolders


CV_STOP_ = Condition()
STOP_ = False


def sigint_signal_handler(signum, frame):
    """SIGINT Signal handler."""
    print(f'Signal handler called with signal {signum}.')

    global STOP_

    # Stop application
    CV_STOP_.acquire()
    STOP_ = True
    CV_STOP_.notify_all()
    CV_STOP_.release()


def run_node(node_name, node_constructor, args=None):

    # TODO input args

    # Create "unique" name
    name = node_name + str(random.randint(0, 1000))

    print(f'Starting {name} execution.')

    # Activate signal handler
    signal.signal(signal.SIGINT, sigint_signal_handler)

    # Check result folder exist
    checkFolders()

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

    print(f'Signal received, stopping node {name}. This could take few seconds.')

    # Stop node
    main_node.stop()

    # Wait for node to terminate
    node_thread.join()

    print(f'Finishing {name} execution.')
