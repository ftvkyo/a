#include <sstream>

#include "a.hpp"


Lexer::Lexer() {
    SPECIAL_CHARS = "()";
}


std::vector<pToken> Lexer::tokenize(std::istream * const input) {
    std::vector<pToken> result;
    TokenKind tk;

    // read tokens until we reach the eof
    // eof will also get put into the result vector
    do {
        pToken tok = get_next_token(input);
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


pToken Lexer::get_next_token(std::istream * const input) {
    std::stringstream out;
    char current;

    // skip all whitespace and check this is not the end of file yet.
    *input >> std::ws;
    if(input->eof()) {
        return TokenEof::make();
    }

    // read the stream symbol by symbol, extracting symbols only when
    // they can be a part of the token we are "growing" in `out`.
    // this allows us avoid having a (static) variable that keeps track
    // of the last character.
    do {
        *input >> current;
        out << current;
    } while(is_long_token_pair(current, input->peek()));

    return token_matcher.match(out.str());
}
