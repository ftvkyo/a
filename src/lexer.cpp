#include <sstream>

#include "a.hpp"


Lexer::Lexer() {
    SPECIAL_CHARS = "()";
}


std::vector<std::unique_ptr<Token>> Lexer::tokenize(std::istream * const input) {
    std::vector<std::unique_ptr<Token>> result;
    TokenKind tk;

    do {
        std::unique_ptr<Token> tok = get_next_token(input);
        tk = tok->kind;
        result.emplace_back(std::move(tok));
    } while(tk != TokenKind::eof);

    return result;
}


bool Lexer::is_long_token_char(char c) {
    const bool is_whitespace = std::isspace(c);
    const bool is_special = SPECIAL_CHARS.find(c) != std::string::npos;
    const bool is_eof = c == EOF;

    return (not is_whitespace) and (not is_special) and (not is_eof);
}


bool Lexer::is_long_token_pair(char left, char right) {
    const bool left_ok = is_long_token_char(left);
    const bool right_ok = is_long_token_char(right);

    return left_ok and right_ok;
}


std::unique_ptr<Token> Lexer::get_next_token(std::istream * const input) {
    std::stringstream out;
    char current;

    *input >> std::ws;
    if(input->eof()) {
        return TokenEof::make();
    }

    do {
        *input >> current;
        out << current;
    } while(is_long_token_pair(current, input->peek()));

    return token_matcher.match(out.str());
}
