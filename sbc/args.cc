#include "args.hh"

#include <optional>

using namespace sb;

static std::optional<std::string_view> shift_arg(int &argc, const char **&argv) {
    if (argc <= 0)
        return {};

    argc--;

    return *argv++;
}

Args Args::parse_args(int argc, const char **argv) {
    Args args{};
    auto help = false;

    args.program = *shift_arg(argc, argv);

    auto arg = shift_arg(argc, argv);

    while (arg.has_value()) {
        if (*arg == "-o" || *arg == "--output") {
            args.output = *shift_arg(argc, argv);
        } else if (*arg == "-c" || *arg == "--compile") {
            args.compile = true;
        } else if (*arg == "-h" || *arg == "--help") {
            help = true;
        } else if (arg->starts_with("-b:")) {
            args.blocks.insert(std::pair(arg->substr(3), *shift_arg(argc, argv)));
        } else if (arg->starts_with("--block:")) {
            args.blocks.insert(std::pair(arg->substr(8), *shift_arg(argc, argv)));
        } else {
            args.inputs.push_back(*arg);
        }

        arg = shift_arg(argc, argv);
    }

    if (help) {
        Args::print_usage(std::cout, args.program);
        std::exit(0);
    }

    if (!args.is_valid()) {
        std::cerr << args << std::endl;
        std::exit(1);
    }

    return args;
}

std::ostream &Args::print_usage(std::ostream &os, std::string_view program) {
    os << "Usage:\t";

    if (std::getenv("BUILD_WORKSPACE_DIRECTORY") != nullptr) {
        os << "bazel run -- @scratch_rules//sbc";
    } else {
        os << program;
    }

    return os << " [-c] [-b:<block> <file,...> ...] -o <output> <inputs...>"
              << std::endl
              << "Flags:" << std::endl
              << "\t-h,--help:\t\tShow this help" << std::endl
              << "\t-o,--output <output>:\tOutput file" << std::endl
              << "\t-b:<block> <file.ndjson>:\tNewline delimited json describing blocks for a package"
              << std::endl
              << "\t-c,--compile:\t\tCompile object file (.sbo). If omitted, it will compile a Scratch executable (.sb3)."
              << std::endl;
}

bool Args::is_valid() const { return errors().size() == 0; }

std::vector<std::string> Args::errors() const {
    if (inputs.size() == 0) {
        return {"Missing input files"};
    }
    if (output.size() == 0) {
        return {"Missing output files"};
    }
    return {};
}

std::ostream &operator<<(std::ostream &os, const Args &args) {
    auto errors{args.errors()};

    if (errors.size() == 0) {
        os << "Args { output = \"" << args.output << "\", inputs = [ ";
        for (auto input : args.inputs) {
            os << "\"" << input << "\" ";
        }
        return os << "], compile = " << args.compile << " }";
    }

    Args::print_usage(os, args.program) << std::endl;

    for (const auto &err : errors) {
        os << "Error: " << err << std::endl;
    }

    return os;
}
