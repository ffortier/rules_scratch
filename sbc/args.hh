#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace sb {
struct Args;
}

std::ostream &operator<<(std::ostream &, const sb::Args &args);

namespace sb {
struct Args {
    static Args parse_args(int argc, const char **argv);
    static std::ostream &print_usage(std::ostream &os, std::string_view program);

    std::string_view program;
    std::string_view output;
    std::vector<std::string_view> inputs;
    std::map<std::string_view, std::string_view> blocks;

    bool compile;

    bool is_valid() const;
    std::vector<std::string> errors() const;

    friend std::ostream &(::operator<<)(std::ostream &, const Args &args);
};
} // namespace sb