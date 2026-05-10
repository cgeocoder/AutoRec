#include <iostream>
#include <vector>
#include <string>

enum class TokenType {
    ID, NUM, ERROR
};

struct Token {
    TokenType type;
    std::string value;
};

class Tokenizer {
private:
    enum class ParseState {
        s, s_id, m_id, s_num, m_num
    };

    static bool allowed_symbol(char _Sym) {
        return (_Sym >= 'a' && _Sym <= 'z')
            || (_Sym >= 'A' && _Sym <= 'Z')
            || (_Sym >= '0' && _Sym <= '9')
            || (_Sym == ' ')
            || (_Sym == '\n')
            || (_Sym == '\0')
            || (_Sym == '\t');
    }

    static inline bool is_start_id(char _Sym) {
        return (_Sym >= 'a' && _Sym <= 'z')
            || (_Sym >= 'A' && _Sym <= 'Z')
            || (_Sym == '_');
    }

    static inline bool is_middle_id(char _Sym) {
        return (_Sym >= 'a' && _Sym <= 'z')
            || (_Sym >= 'A' && _Sym <= 'Z')
            || (_Sym >= '0' && _Sym <= '9')
            || (_Sym == '_');
    }

    static inline bool is_start_num(char _Sym) {
        return (_Sym >= '0' && _Sym <= '9');
    }

    static inline bool is_middle_num(char _Sym) {
        return is_start_num(_Sym);
    }

    static inline bool is_separator(char _Sym) {
        return (_Sym == ' ')
            || (_Sym == '\n')
            || (_Sym == '\t');
    }

    static inline bool is_end(char _Sym) {
        return _Sym == '\0';
    }

public:
    static std::vector<Token> parse(const std::string& _String) {
        std::string _Str = _String + '\0';

        ParseState state = ParseState::s;
        std::vector<Token> result;
        size_t start_tok_pos = 0;

        for (size_t i = 0; i < _Str.length(); ++i) {
            char ch = _Str.at(i);

            if (!allowed_symbol(ch)) {
                std::cout << "Error: disallowed symbol '" << ch << "'\n";
                return result;
            }

            switch (state)
            {
            case Tokenizer::ParseState::s:
                start_tok_pos = i;

                if (is_start_id(ch)) {
                    state = Tokenizer::ParseState::s_id;
                }
                else if (is_start_num(ch)) {
                    state = Tokenizer::ParseState::s_num;
                }
                else if (is_separator(ch)) {
                    state = Tokenizer::ParseState::s;
                }
                else if (is_end(ch)) {
                    return result;
                }
                else {
                    std::cout << "Error: disallowed character sequence\n";
                    return result;
                }

                break;
            case Tokenizer::ParseState::s_id:
            case Tokenizer::ParseState::m_id:
                if (is_separator(ch)) {
                    state = Tokenizer::ParseState::s;

                    result.push_back({
                        TokenType::ID, _Str.substr(start_tok_pos, i - start_tok_pos)
                        });
                }
                else if (is_middle_id(ch)) {
                    state = Tokenizer::ParseState::m_id;
                }
                else if (is_end(ch)) {
                    result.push_back({
                        TokenType::ID, _Str.substr(start_tok_pos, i - start_tok_pos)
                        });

                    return result;
                }
                else {
                    std::cout << "Error: disallowed character sequence\n";
                    return result;
                }
                break;

            case Tokenizer::ParseState::s_num:
            case Tokenizer::ParseState::m_num:
                if (is_separator(ch)) {
                    state = Tokenizer::ParseState::s;

                    result.push_back({
                        TokenType::NUM, _Str.substr(start_tok_pos, i - start_tok_pos)
                        });
                }
                else if (is_middle_num(ch)) {
                    state = Tokenizer::ParseState::m_num;
                }
                else if (is_end(ch)) {
                    result.push_back({
                        TokenType::NUM, _Str.substr(start_tok_pos, i - start_tok_pos)
                        });

                    return result;
                }
                else {
                    std::cout << "Error: disallowed character sequence\n";
                    return result;
                }

                break;
            }
        }

        return result;
    }

    static void print_tokens(const std::vector<Token> _Toks) {
        if (_Toks.empty()) {
            std::cout << "\n";
            return;
        }

        std::cout << "[\n";

        auto token_type_to_str = [&](const Token& tok) -> std::string {
            if (tok.type == TokenType::ID)
                return "id";
            else if (tok.type == TokenType::NUM)
                return "num";
            else 
                return "<unknown>";
        };

        for (size_t i = 0; i < _Toks.size(); ++i) {
            const Token& tok = _Toks.at(i);

            std::cout << "\t(" << token_type_to_str(tok) << ": '" << tok.value << "')";

            if (i != _Toks.size() - 1) {
                std::cout << ",\n";
            }
            else {
                std::cout << "\n";
            }
        }

        std::cout << "]\n";
    }
};

int main() {
    std::vector<std::string> test = {
        "123 456",
        "   hello\tworld   ",
        "123as23s",
        "C++ is good language",
        "  print a 1    2 3 4",
    };

    for (auto& str : test) {
        std::cout << "'" << str << "' -> ";
        Tokenizer::print_tokens(Tokenizer::parse(str));
        std::cout << "\n";
    }
}
