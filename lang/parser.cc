#include "parser.hh"

#include <glog/logging.h>

using namespace sb;

static std::unique_ptr<Expr> parse_expr(auto &iter, auto &end);

static std::unique_ptr<Expr> parse_list_expr(auto &iter, auto &end) {
    std::vector<std::unique_ptr<Expr>> items{};
    ++iter;
    while (iter != end && iter->type != TokenType::CloseParen) {
        items.push_back(parse_expr(iter, end));
    }
    ++iter;
    return std::make_unique<Expr>(ListExpr{std::move(items)});
}

static std::unique_ptr<Expr> parse_ident_expr(auto &iter, auto &end) {
    std::string first{iter->text};

    ++iter;

    if (iter->type == TokenType::Dot) {
        ++iter;
        std::string second{iter->text};
        ++iter;

        return std::make_unique<Expr>(IdentExpr{first, second});
    }

    return std::make_unique<Expr>(IdentExpr{first});
}

static std::unique_ptr<Expr> parse_expr(auto &iter, auto &end) {
    switch (iter->type) {
    case TokenType::OpenParen:
        return parse_list_expr(iter, end);
    case TokenType::Atom:
        return parse_ident_expr(iter, end);
    default:
        CHECK(false) << "Unexpected token: " << *iter << std::endl;
        return nullptr;
    }
}

// static auto parse_top_level_block(Parser::Iterator &current, Parser::Iterator &end) {}

std::unique_ptr<Expr> Parser::parse(Iterator &current, Iterator &end) {
    while (current != end) {
        parse_expr(current, end);
    }

    return std::make_unique<Expr>(ListExpr{});
}

Parser::Parser() : blocks{{}} {}
