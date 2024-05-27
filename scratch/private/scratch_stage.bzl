ScratchStageInfo = provider(
    "Scratch Stage",
    fields = {},
)

def _impl(ctx):
    return [
        ScratchStageInfo(),
    ]

scratch_stage = rule(
    implementation = _impl,
    attrs = {
        "srcs": attr.label_list(allow_files = [".lisp", ".lsp"]),
        "backdrops": attr.label_list(allow_files = [".svg", ".png"]),
    },
    provides = [ScratchStageInfo],
)
