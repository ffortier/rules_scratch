#include <gtest/gtest.h>
#include <ranges>

#include "lexer.hh"

using namespace sb;

TEST(Lexer, Tokens) {
    auto lisp{"(some.test (+ life \"universe everything\") 42)"};
    auto tokens{lexer(lisp)};

    EXPECT_EQ(tokens.size(), 11);
    EXPECT_TRUE(tokens[0].text == "(") << "but was: " << tokens[0].text;
    EXPECT_TRUE(tokens[1].text == "some") << "but was: " << tokens[1].text;
    EXPECT_TRUE(tokens[2].text == ".") << "but was: " << tokens[2].text;
    EXPECT_TRUE(tokens[3].text == "test") << "but was: " << tokens[3].text;
    EXPECT_TRUE(tokens[4].text == "(") << "but was: " << tokens[4].text;
    EXPECT_TRUE(tokens[5].text == "+") << "but was: " << tokens[5].text;
    EXPECT_TRUE(tokens[6].text == "life") << "but was: " << tokens[6].text;
    EXPECT_TRUE(tokens[7].text == "\"universe everything\"") << "but was: " << tokens[7].text;
    EXPECT_TRUE(tokens[8].text == ")") << "but was: " << tokens[8].text;
    EXPECT_TRUE(tokens[9].text == "42") << "but was: " << tokens[9].text;
    EXPECT_TRUE(tokens[10].text == ")") << "but was: " << tokens[10].text;
}