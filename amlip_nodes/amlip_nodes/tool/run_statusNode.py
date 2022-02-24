"""Run Aml Status Node."""

from amlip_nodes.aml.aml_statusNode import StatusNode
from amlip_nodes.tool.run_node_common import parse_args, run_node


def main(args=None):
    """Run main routine with Status node."""
    domain, seed, debug_level = parse_args()
    run_node(
        node_name='AML-IP Status Node',
        node_constructor=StatusNode,
        domain=domain,
        seed=seed,
        debug_level=debug_level)


if __name__ == '__main__':
    main()
