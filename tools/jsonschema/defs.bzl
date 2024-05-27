load("@npm//tools/jsonschema:quicktype/package_json.bzl", "bin")
load("@rules_cc//cc:defs.bzl", "cc_library")

def jsonschema_cc_library(name, srcs, main, namespace = None, **kwargs):
    bin.quicktype(
        name = "%s_hdrs" % name,
        srcs = srcs,
        outs = ["%s.hh" % name],
        args = [
            "--lang",
            "c++",
            "--namespace",
            namespace or name,
            "--code-format",
            "with-struct",
            "-s",
            "schema",
            "--no-boost",
            "-o",
            "%s.hh" % name,
            main,
        ],
        chdir = native.package_name(),
    )

    cc_library(
        name = name,
        hdrs = [":%s_hdrs" % name],
        deps = ["//:nlohmann_json"],
        **kwargs
    )
