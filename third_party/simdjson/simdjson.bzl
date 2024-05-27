RELEASES = {
    "3.9.1": {
        "simdjson.cpp": struct(
            urls = ["https://github.com/simdjson/simdjson/releases/download/v3.9.1/simdjson.cpp"],
            sha256 = "15f47fcc4ff4e5ad11ff369a142046b43f7aec2dd46161a5a2fd3d961c15cb4b",
        ),
        "simdjson.h": struct(
            urls = ["https://github.com/simdjson/simdjson/releases/download/v3.9.1/simdjson.h"],
            sha256 = "ca9b94f00a5ed99da6eb84ecc0b9c139be8a9cc95f8edc73395b1953aea8e31e",
        ),
    },
}

LATEST_RELEASE = "3.9.1"

def _impl(rctx):
    release = RELEASES[rctx.attr.release]

    if not release:
        fail("Could not find simdjson release %s" % rctx.attr.release)

    rctx.download(
        url = release["simdjson.cpp"].urls,
        sha256 = release["simdjson.cpp"].sha256,
        output = "simdjson.cpp",
    )

    rctx.download(
        url = release["simdjson.h"].urls,
        sha256 = release["simdjson.h"].sha256,
        output = "simdjson.h",
    )

    rctx.template("BUILD.bazel", rctx.attr._build_file, executable = False)

simdjson = repository_rule(
    implementation = _impl,
    attrs = {
        "release": attr.string(default = LATEST_RELEASE),
        "_build_file": attr.label(default = "BUILD.simdjson.bazel", allow_single_file = True),
    },
)
