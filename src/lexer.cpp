#include "a.hpp"


TokenMatcher::TokenMatcher() {
    token_matches.emplace_back([](std::string s) {
        return s == "(";
    }, TokenKind::bracket_left);

    token_matches.emplace_back([](std::string s) {
        return s == ")";
    }, TokenKind::bracket_right);

    token_matches.emplace_back([](std::string s) {
        for(auto it = s.begin(); it < s.end(); it++) {
            if(not std::isdigit(*it)) {
                return false;
            }
        }
        return true;
    }, TokenKind::integer);

    token_matches.emplace_back([](std::string s) {
        for(auto it = s.begin(); it < s.end(); it++) {
            if(not std::isgraph(*it)) {
                return false;
            }
        }
        return true;
    }, TokenKind::identifier);
}


TokenKind TokenMatcher::match(std::string token) {
    for(auto it = token_matches.begin(); it < token_matches.end(); it++) {
        if(it->first(token)) {
            return it->second;
        }
    }

    throw SyntaxError();
}


Lexer::Lexer() {
    SPECIAL_CHARS = "()";
}


std::vector<TokenKind> Lexer::tokenize(std::istream * const input) {
    std::vector<TokenKind> result;
    TokenKind tok = get_next_token(input);
    result.push_back(tok);

    while(tok != TokenKind::eof) {
        tok = get_next_token(input);
        result.push_back(tok);
    }

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


TokenKind Lexer::get_next_token(std::istream * const input) {
    std::stringstream out;
    char current;

    *input >> std::ws;
    if(input->eof()) {
        return TokenKind::eof;
    }

    do {
        *input >> current;
        out << current;
    } while(is_long_token_pair(current, input->peek()));

    return token_matcher.match(out.str());
}
