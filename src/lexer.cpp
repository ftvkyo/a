#include "a.hpp"


Lexer::Lexer() = default;

std::vector<Token> Lexer::tokenize(std::istream const * input) {
    (void) input;
    return {Token::eof};
}
