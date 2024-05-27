ScratchSpriteInfo = provider(
    "Scratch Sprite",
    fields = {},
)

def _impl(ctx):
    scratch_toolchain = ctx.toolchains["//scratch:toolchain_type"]
    output = ctx.actions.declare_file("%s.sbo" % ctx.label.name)

    args = ctx.actions.args()

    args.add("-c")
    args.add("-o").add(output)
    args.add_all(ctx.files.srcs)

    for block_package, block_file in scratch_toolchain.blocks.items():
        args.add("-b:%s" % block_package).add(block_file)

    ctx.actions.run(
        executable = scratch_toolchain.tools.sbc,
        arguments = [args],
        inputs = ctx.files.srcs + ctx.files.costumes + scratch_toolchain.blocks.values(),
        outputs = [output],
    )

    return [
        ScratchSpriteInfo(),
        DefaultInfo(
            files = depset([output]),
        ),
    ]

scratch_sprite = rule(
    implementation = _impl,
    attrs = {
        "srcs": attr.label_list(allow_files = [".lisp", ".lsp"]),
        "costumes": attr.label_list(allow_files = [".svg", ".png"]),
    },
    provides = [ScratchSpriteInfo],
    toolchains = ["//scratch:toolchain_type"],
)
