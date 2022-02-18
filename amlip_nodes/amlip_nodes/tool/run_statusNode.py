"""Run Aml Status Node."""

from amlip_nodes.aml.aml_statusNode import StatusNode
from amlip_nodes.tool.run_node_common import run_node


def main(args=None):
    """Run main routine with Status node."""
    run_node('AML-IP Status Node', StatusNode)


if __name__ == '__main__':
    main()
