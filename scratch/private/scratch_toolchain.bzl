load(":private/scratch_blocks.bzl", "ScratchBlocksInfo")

def _impl(ctx):
    return [
        platform_common.ToolchainInfo(
            tools = struct(
                sbc = ctx.executable.sbc,
            ),
            blocks = {
                target[ScratchBlocksInfo].block_package: target[ScratchBlocksInfo].file
                for target in ctx.attr.blocks
            },
        ),
    ]

scratch_toolchain = rule(
    implementation = _impl,
    attrs = {
        "sbc": attr.label(executable = True, cfg = "exec", default = "//sbc"),
        "blocks": attr.label_list(providers = [ScratchBlocksInfo], default = ["//blocks:std"]),
    },
)
