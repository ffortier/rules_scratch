load(":private/scratch_sprite.bzl", "ScratchSpriteInfo")
load(":private/scratch_stage.bzl", "ScratchStageInfo")

def _impl(ctx):
    pass

scratch_binary = rule(
    implementation = _impl,
    attrs = {
        "sprites": attr.label_list(providers = [ScratchSpriteInfo]),
        "stage": attr.label(providers = [ScratchStageInfo], mandatory = True),
    },
)
