#include "a.hpp"


Lexer::Lexer() = default;

std::vector<Token> Lexer::tokenize(std::istream * const input) {
    std::vector<Token> result;
    Token tok = get_next_token(input);
    result.push_back(tok);

    while(tok != Token::eof) {
        tok = get_next_token(input);
        result.push_back(tok);
    }

    return result;
}

bool Lexer::are_from_the_same_token(char a, char b) {
    (void) a;
    (void) b;
    return false;
}

Token Lexer::get_next_token(std::istream * const input) {
    std::stringstream out;
    char current;

    (*input) >> std::ws;

    while(are_from_the_same_token(current, input->peek())) {
        out << current;
        (*input) >> current;
    }
    out << current;

    std::cout << "Token: " << out.str() << std::endl;

    return Token::eof;
}
