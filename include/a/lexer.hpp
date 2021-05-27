#pragma once

#include <iostream>
#include <vector>


enum class Token {
    eof = -1,

    bracket_left = -2,
    bracket_right = -3,

    bracket_square_left = -4,
    bracket_square_right = -5,

    identifier = -6,

    number = -7,
    string = -8,
};


class Lexer {
public:
    Lexer();

    std::vector<Token> tokenize(std::istream const * input);
};
