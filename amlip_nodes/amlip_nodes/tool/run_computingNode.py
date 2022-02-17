"""Run Aml Computing Node."""

from amlip_nodes.aml.aml_computingNode import ComputingNode
from amlip_nodes.tool.run_node_common import run_node


def main(args=None):
    """Run main routine with Computing node."""
    run_node('AML-IP Computing Node', ComputingNode)


if __name__ == '__main__':
    main()
