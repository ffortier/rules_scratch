#include "args.hh"
#include "blocks/schema.hh"
#include "json.hpp"
#include <ZipLib/ZipArchiveEntry.h>
#include <ZipLib/ZipFile.h>
#include <fstream>
#include <glog/logging.h>
#include <iostream>
#include <md5/md5.h>
#include <ranges>

using namespace sb;
using json = nlohmann::json;

void read_blocks(std::string_view file_path, std::string prefix) {
    std::ifstream std_block_stream(file_path);

    if (!std_block_stream) {
        std::cerr << "Could not open file " << file_path << ": " << strerror(errno) << std::endl;
        std::exit(1);
    }

    for (std::string line; std::getline(std_block_stream, line);) {
        auto json_data = json::parse(line);
        auto schema = json_data.get<sb::schema::Schema>();
    }
}

std::string hash_image(std::string_view file_path) {
    std::vector<char> buffer(4096, 0);
    md5::md5_t md5;

    std::ifstream input(file_path, std::ios::binary);

    if (!input) {
        std::cerr << "Could not open file " << file_path << ": " << strerror(errno) << std::endl;
        std::exit(1);
    }

    while (!input.eof()) {
        input.read(buffer.data(), buffer.size());
        auto cb = input.gcount();

        md5.process(buffer.data(), cb);
    }

    md5.finish();

    std::string md5_str(MD5_SIZE * 2, '\0');

    md5.get_string(md5_str.data());

    return md5_str;
}

int main(int argc, const char **argv) {
    google::InitGoogleLogging(argv[0]);

    auto args{Args::parse_args(argc, argv)};

    std::cout
        << args.blocks.at("std") << std::endl
        << args.output << std::endl;

    for (auto [key, value] : args.blocks) {
        read_blocks(value, key == "std" ? "" : std::string(key) + ".");
    }

    for (auto file_path : args.inputs) {
        std::cout << file_path << ":" << hash_image(file_path) << std::endl;
    }
}