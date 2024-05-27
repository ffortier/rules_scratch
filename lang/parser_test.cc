#include <gtest/gtest.h>
#include <ranges>

#include "parser.hh"

using namespace sb;

TEST(Parser, TopLevel) {
    Location loc{"<source>", "nothing", 1, 1, 0};
    std::vector<Token> tokens{
        Token{.type = TokenType::OpenParen, .loc = loc, .text = "("},
        Token{.type = TokenType::Atom, .loc = loc, .text = "pen"},
        Token{.type = TokenType::Dot, .loc = loc, .text = "."},
        Token{.type = TokenType::Atom, .loc = loc, .text = "penDown"},
        Token{.type = TokenType::CloseParen, .loc = loc, .text = ")"},
    };

    Parser parser{};

    auto ast = parser.parse(tokens);
}