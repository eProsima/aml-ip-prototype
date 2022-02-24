"""Run Aml Computing Node."""

import argparse
from distutils.log import debug
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


def parse_args():
    """Parse the executable arguments."""
    parser = argparse.ArgumentParser(description='Process some integers.')

    parser.add_argument('-d', '--domain', default=0, type=int,
                        help='DDS Participant Domain Id.')

    parser.add_argument('--debug', action='store_true',
                        help='Activate debug traces.')

    parser.add_argument('-s', '--seed', default=666, type=int,
                        help='Random seed.')

    args = parser.parse_args()

    debug_level = logging.USER
    if args.debug:
        debug_level = logging.ARCH

    return (args.domain, args.seed, debug_level)


def run_node(
        node_name,
        node_constructor,
        domain=0,
        seed=666,
        debug_level=logging.USER):
    """TODO comment."""
    # Allow any log level
    logging.basicConfig(level=logging.NOTSET - 10)
    # Only show above this level
    logger.setLevel(debug_level)

    # TODO input args

    # Set random seed
    random.seed(seed)
    logger.info(f'Using random seed {seed}')

    # Create "unique" name
    name = node_name + ' ' + str(random.randint(0, 999)).zfill(3)

    logger.user(f'Starting {name} execution.')

    # Activate signal handler
    signal.signal(signal.SIGINT, sigint_signal_handler)

    # Check result folder exist
    # checkFolders()

    # Create node and run
    main_node = node_constructor(
        name=name,
        domain=domain)

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
