#include "lexer.hh"

using namespace sb;

double Token::as_double() const { return 0.0; }

std::string Token::as_string() const { return ""; }

static std::string_view read_number(std::string_view source, size_t start) {
    auto end = start + 1;

    while (source[end] >= '0' && source[end] <= '9') {
        end += 1;
    }

    if (source[end] == '.') {
        end += 1;

        while (source[end] >= '0' && source[end] <= '9') {
            end += 1;
        }
    }

    return source.substr(start, end - start);
}

static std::string_view read_string(std::string_view source, size_t start) {
    auto end = start + 1;

    while (source[end] != '"') {
        if (source[end++] == '\\') {
            end += 1;
        }
    }

    end += 1;

    return source.substr(start, end - start);
}

static std::string_view read_atom(std::string_view source, size_t start) {
    auto end = start + 1;

    while (std::isalnum(source[end])) {
        end += 1;
    }

    return source.substr(start, end - start);
}

std::ostream &operator<<(std::ostream &os, const sb::Token &token) {
    return os << token.loc << token.text;
}

std::ostream &operator<<(std::ostream &os, const sb::Location &loc) {
    return os << loc.file_path << ":" << loc.ln << ":" << loc.col << ": ";
}

namespace sb {
std::vector<Token> lexer(std::string_view source, std::string_view file_path) {
    std::vector<Token> tokens{};
    size_t ln = 1;
    size_t col = 1;
    size_t i = 0;
    size_t len;
    std::string_view text;

    while (i < source.length()) {
        switch (source[i]) {
        case '(':
            tokens.push_back(Token{
                .type = TokenType::OpenParen,
                .loc = Location{file_path, source, col, ln, i},
                .text = source.substr(i, 1),
            });
            i += 1;
            break;
        case ')':
            tokens.push_back(Token{
                .type = TokenType::CloseParen,
                .loc = Location{file_path, source, col, ln, i},
                .text = source.substr(i, 1),
            });
            i += 1;
            break;
        case '.':
            tokens.push_back(Token{
                .type = TokenType::Dot,
                .loc = Location{file_path, source, col, ln, i},
                .text = source.substr(i, 1),
            });
            i += 1;
            break;
        case '"':
            text = read_string(source, i);
            len = text.length();
            tokens.push_back(Token{
                .type = TokenType::Number,
                .loc = Location{file_path, source, col, ln, i},
                .text = std::move(text),
            });
            i += len;
            break;
        case '\n':
            i += 1;
            ln += 1;
            col = 0;
            break;
        case '\t':
        case '\r':
        case ' ':
            i += 1;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            text = read_number(source, i);
            len = text.length();
            tokens.push_back(Token{
                .type = TokenType::Number,
                .loc = Location{file_path, source, col, ln, i},
                .text = std::move(text),
            });
            i += len;
            break;
        default:
            text = read_atom(source, i);
            len = text.length();
            tokens.push_back(Token{
                .type = TokenType::Atom,
                .loc = Location{file_path, source, col, ln, i},
                .text = std::move(text),
            });
            i += len;
            break;
        }
        col += 1;
    }

    return tokens;
}
} // namespace sb