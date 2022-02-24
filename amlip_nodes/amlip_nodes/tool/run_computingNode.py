"""Run Aml Computing Node."""

from amlip_nodes.aml.aml_computingNode import ComputingNode
from amlip_nodes.tool.run_node_common import parse_args, run_node


def main(args=None):
    """Run main routine with Computing node."""
    domain, seed, debug_level = parse_args()
    run_node(
        node_name='AML-IP Computing Node',
        node_constructor=ComputingNode,
        domain=domain,
        seed=seed,
        debug_level=debug_level)


if __name__ == '__main__':
    main()
