
from time import sleep

from amlip_nodes.dds.node.AmlNode import AmlNode
from amlip_nodes.dds.node.MainNode import MainNode

if __name__ == '__main__':

    print('Starting execution')

    print('Creating Node')
    node = MainNode('MainNode')
    sleep(1)

    print('Starting Node')
    node.init()
    print('Waiting execution')
    sleep(5)

    print('Ending Node')
    node.stop()

    print('Finishing execution')
