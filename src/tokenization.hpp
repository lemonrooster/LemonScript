#pragma once

#include <string>
#include <vector>
#include <optional>
#include <cctype>

enum class TokenType {
    exit,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
public:
    explicit Tokenizer(std::string src)
        : m_src(std::move(src))
    {
    }

    std::vector<Token> tokenize() {
        std::string buf;
        std::vector<Token> tokens;
        while (peek().has_value()) {
            if (std::isalpha(peek().value())) {
                buf.push_back(consume());
                while (peek().has_value() && std::isalnum(peek().value())) {
                    buf.push_back(consume());
                }
                if (buf == "exit") {
                    tokens.push_back({TokenType::exit});
                } else {
                    std::cerr << "Unrecognized identifier at index " << m_index << ": " << buf << std::endl;
                    exit(EXIT_FAILURE);
                }
                buf.clear();
            } else if (std::isdigit(peek().value())) {
                buf.push_back(consume());
                while (peek().has_value() && std::isdigit(peek().value())) {
                    buf.push_back(consume());
                }
                tokens.push_back({TokenType::int_lit, buf});
                buf.clear();
                continue;
            }
            else if (peek().value() == ';') {
                consume(); // Consume the ';' character
                tokens.push_back({TokenType::semi});
                continue;
            }
            else if (std::isspace(peek().value())) {
                consume(); // Consume the whitespace character
                continue;
            }
            else {
                std::cerr << "Unexpected character at index " << m_index << ": " << peek().value() << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return tokens;
    }

private:
    [[nodiscard]] inline std::optional<char> peek(int ahead = 0) const {
        if (m_index + ahead >= m_src.length()) {
            return {};
        }
        return m_src[m_index + ahead];
    }

    inline char consume() {
        return m_src[m_index++];
    }

    const std::string m_src;
    size_t m_index = 0;
};