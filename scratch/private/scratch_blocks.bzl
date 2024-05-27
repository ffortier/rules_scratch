load("//tools/stdcapture:defs.bzl", "stdcapture_action")

ScratchBlocksInfo = provider(
    "Scratch Blocks",
    fields = {
        "block_package": "the name of the block package",
        "file": "The ndjson file containing the block definitions",
    },
)

def _impl(ctx):
    output = ctx.actions.declare_file(ctx.label.name + ".ndjson")
    jq = ctx.toolchains["@aspect_bazel_lib//lib:jq_toolchain_type"].jqinfo

    files = ctx.files.srcs + [dep[ScratchBlocksInfo].file for dep in ctx.attr.deps]

    if len(files) == 0:
        ctx.actions.write(
            output = output,
            content = "",
            is_executable = False,
        )
    else:
        args = []
        args.append("-r")
        args.append("del(.[\"$schema\"]) | tostring")
        args.extend(files)

        # if len(args) > 2:
        stdcapture_action(
            ctx,
            executable = jq.bin,
            inputs = files,
            stdout = output,
            arguments = args,
            stdcapture = ctx.executable._stdcapture,
        )

    return [
        DefaultInfo(files = depset([output])),
        ScratchBlocksInfo(
            block_package = ctx.attr.block_package or ctx.label.name,
            file = output,
        ),
    ]

scratch_blocks = rule(
    implementation = _impl,
    attrs = {
        "srcs": attr.label_list(allow_files = [".json"]),
        "deps": attr.label_list(providers = [ScratchBlocksInfo]),
        "block_package": attr.string(),
        "_stdcapture": attr.label(executable = True, default = "//tools/stdcapture", cfg = "exec"),
    },
    provides = [ScratchBlocksInfo],
    toolchains = ["@aspect_bazel_lib//lib:jq_toolchain_type"],
)
