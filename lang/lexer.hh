#pragma once

#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>

namespace sb {
struct Token;
struct Location;
} // namespace sb

std::ostream &operator<<(std::ostream &os, const sb::Token &token);
std::ostream &operator<<(std::ostream &os, const sb::Location &loc);

namespace sb {
enum class TokenType {
    OpenParen,
    CloseParen,
    Atom,
    String,
    Number,
    Dot,
};

struct Location {
    std::string_view file_path;
    std::string_view source;
    size_t col;
    size_t ln;
    size_t offset;
};

struct Token {
    TokenType type;
    Location loc;
    std::string_view text;

    std::string as_string() const;
    double as_double() const;
};

std::vector<Token> lexer(std::string_view source, std::string_view file_path = "<source>");
} // namespace sb