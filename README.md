# Rules Scratch

**WORK IN PROGRESS**

I just started, it's not functional yet

## Usage

```python
# MODULE.bzl
bazel_dep(name = "rules_scratch")

archive_override(
    module_name = "rules_scratch",
    urls = [...]
    integrity = "sha256:..."
    strip_prefix = "..."
)
```

It is also recommended to use a hermetic llvm toolchain that support c++20 standard

```python
bazel_dep(name = "toolchains_llvm", version = "1.0.0", dev_dependency = True)

llvm = use_extension("@toolchains_llvm//toolchain/extensions:llvm.bzl", "llvm")

llvm.toolchain(
    llvm_version = "16.0.0",
    cxx_standard = {
        "darwin-aarch64": "c++20",
        "darwin-x86_64": "c++20",
        "linux-aarch64": "c++20",
        "linux-x86_64": "c++20",
    },
)

use_repo(llvm, "llvm_toolchain")
```

## Development

*Disclamer* Rust is a superior language but I wanted to try c++20 so that's how I built all the tooling. 

```shell
bazel run @hedron_compile_commands//:refresh_all
```

## TODO

* [x] Start something
* [ ] Lisp inspired language
  * [ ] Compile to sb3
  * [ ] Support internationalization
  * [ ] Platform constraints
* [ ] Runtime
  * [ ] Scratch Classic
  * [ ] Turbowrap
  * [ ] Turbowrap Native
  * [ ] Leopardjs
