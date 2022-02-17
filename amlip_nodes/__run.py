
from time import sleep
from threading import Thread

from amlip_nodes.aml.aml_mainNode import MainNode

if __name__ == '__main__':

    print('Starting execution')

    print('Creating Node')
    node = MainNode('AmlMainNode')
    sleep(1)

    print('Starting Node')
    node_thread = Thread(target=node.run)
    node_thread.start()

    print('Waiting execution')
    sleep(5)

    print('Ending Node')
    node.stop()

    print('Waiting Node finalization')
    node_thread.join()

    print('Finishing execution')
