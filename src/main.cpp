#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <optional>

enum class TokenType {
    _return,
    int_lit,
    semi
};
struct Token {
    TokenType type;
    std::optional<std::string> value;
};

std::vector<Token> tokenize(const std::string &str) {
    std::vector<Token> tokens{};
    std::string buf;
    for (int i = 0; i < str.length(); i++) {
        char c = str.at(i);
        if (std::isalpha(c)) {
            buf.push_back(c);
            i++;
            while (i < str.length() && std::isalnum(str.at(i))) {
                buf.push_back(str.at(i));
                i++;
            }
            if (buf == "return") {
                tokens.push_back({.type =TokenType::_return});
            }
            buf.clear();
        } else if (std::isspace(c)) {
            continue;
        } else if (std::isdigit(c)) {
            buf.push_back(c);
            i++;
            while (i < str.length() && std::isdigit(str.at(i))) {
                buf.push_back(str.at(i));
                i++;
            }
            tokens.push_back({.type= TokenType::int_lit, .value = buf});
            buf.clear();
        } else if (c == ';') {
            tokens.push_back({.type = TokenType::semi});
        } else {
            std::cerr << "Ya Messed Up" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (i < str.length() && str.at(i) == ';') {
            tokens.push_back({.type = TokenType::semi});
            i++;
        }
    }
    return tokens;
}

std::string tokens_to_asm(std::vector<Token> &tokens) {
    std::stringstream output;
    output << "global_start\n_start:\n";
    for (int i = 0; i < tokens.size(); i++) {
        const Token &token = tokens.at(i);
        if (token.type == TokenType::_return) {
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit) {
                if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
                    output << "    mov rax, 60\n";
                    output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
                    output << "    syscall\n";
                }
            }
        }
    }
    return output.str();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect Usage" << std::endl;
        std::cerr << "./lem ../<input.lemon>" << std::endl;
        return EXIT_FAILURE;
    }
    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }
    std::vector<Token> tokens = tokenize(contents);
    {
        std::fstream file("../out.asm", std::ios::out);
        file << tokens_to_asm(tokens);
    }
    std::cout << tokens_to_asm(tokens) << std::endl;
    return EXIT_SUCCESS;
}
