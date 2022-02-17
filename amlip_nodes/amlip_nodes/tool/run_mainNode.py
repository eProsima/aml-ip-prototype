"""Run Aml Main Node."""

from amlip_nodes.aml.aml_mainNode import MainNode
from amlip_nodes.tool.run_node_common import run_node


def main(args=None):
    """Run main routine with Main node."""
    run_node('AML-IP Main Node', MainNode)


if __name__ == '__main__':
    main()
