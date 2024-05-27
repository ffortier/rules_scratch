load(":private/scratch_binary.bzl", _scratch_binary = "scratch_binary")
load(":private/scratch_blocks.bzl", _scratch_blocks = "scratch_blocks")
load(":private/scratch_sprite.bzl", _scratch_sprite = "scratch_sprite")
load(":private/scratch_stage.bzl", _scratch_stage = "scratch_stage")
load(":private/scratch_toolchain.bzl", _scratch_toolchain = "scratch_toolchain")

scratch_sprite = _scratch_sprite
scratch_stage = _scratch_stage
scratch_binary = _scratch_binary
scratch_blocks = _scratch_blocks
scratch_toolchain = _scratch_toolchain
