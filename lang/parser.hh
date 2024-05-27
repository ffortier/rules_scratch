#pragma once

#include <concepts>
#include <optional>
#include <ranges>
#include <unordered_map>
#include <vector>

#include "lexer.hh"

namespace sb {
struct Expr {
    Expr() = default;
    Expr(Expr &&) = default;
    virtual ~Expr() = default;
};

struct BlockExpr : public Expr {
    std::string full_name;
    std::string_view package;
    std::string_view name;
};

struct StatementBlockExpr : public BlockExpr {};

struct ContainerBlockExpr : public BlockExpr {
    std::vector<StatementBlockExpr> statements;
};

struct NumberExpr : public Expr {
    double value;
};

struct StringExpr : public Expr {
    std::string value;
};

struct ListExpr : public Expr {
    std::vector<std::unique_ptr<Expr>> items;

    ListExpr() = default;
    ListExpr(ListExpr &&) = default;
    ListExpr(std::vector<std::unique_ptr<Expr>> &&items)
        : Expr{}, items{std::move(items)} {}
};

struct IdentExpr : public Expr {
    std::optional<std::string> package;
    std::string name;

    IdentExpr() = default;
    IdentExpr(IdentExpr &&) = default;
    IdentExpr(std::string package, std::string name)
        : Expr{}, package{std::move(package)}, name{std::move(name)} {}
    IdentExpr(std::string name) : Expr{}, package{}, name{std::move(name)} {}
};

template <typename T>
concept TokenStream =
    std::ranges::input_range<T> && std::ranges::common_range<T> &&
    std::same_as<std::iter_value_t<T>, Token>;

struct Block {
    std::optional<std::string> alias_for;
    bool top_level;
    bool container;
};

class Parser {
  private:
    struct Iterator {
        virtual Iterator &operator++() = 0;
        virtual Token &operator*() = 0;
        virtual Token *operator->() = 0;
        virtual bool equals(const Iterator &other) const = 0;
        virtual ~Iterator() = default;
        Iterator() = default;

        friend bool operator==(const Iterator &a, const Iterator &b) {
            return a.equals(b);
        }
    };

    template <class T>
    struct IteratorImpl : public Iterator {
        T t;

        IteratorImpl(T t) : Iterator{}, t{t} {}

        virtual Iterator &operator++() override {
            ++t;
            return *this;
        }

        virtual Token &operator*() override { return *t; }

        virtual Token *operator->() override { return &*t; }

        virtual bool equals(const Iterator &other) const override {
            auto impl = dynamic_cast<const IteratorImpl<T> *>(&other);

            if (impl == nullptr) {
                return false;
            }

            return impl->t == this->t;
        }
    };

    std::unordered_map<std::string, Block> blocks;

    std::unique_ptr<Expr> parse(Iterator &begin, Iterator &end);
    std::unique_ptr<Expr> parse_top_level_block(Iterator &begin, Iterator &end);
    std::unique_ptr<Expr> parse_block_definition(Iterator &begin, Iterator &end);
    std::unique_ptr<Expr> parse_list_expr(Iterator &begin, Iterator &end);

  public:
    Parser();

    std::unique_ptr<Expr> parse(TokenStream auto &tokens) {
        IteratorImpl wbegin{tokens.begin()};
        IteratorImpl wend{tokens.end()};
        return parse(wbegin, wend);
    }
};
} // namespace sb
