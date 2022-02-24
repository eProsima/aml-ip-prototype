"""Run Aml Main Node."""

from amlip_nodes.aml.aml_mainNode import MainNode
from amlip_nodes.tool.run_node_common import parse_args, run_node


def main(args=None):
    """Run main routine with Main node."""
    domain, seed, debug_level = parse_args()
    run_node(
        node_name='AML-IP Main Node',
        node_constructor=MainNode,
        domain=domain,
        seed=seed,
        debug_level=debug_level)


if __name__ == '__main__':
    main()
