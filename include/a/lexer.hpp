#pragma once

#include <iostream>
#include <sstream>
#include <vector>


enum class Token {
    eof,

    bracket_left,
    bracket_right,

    identifier,

    number,
};


class Lexer {
public:

    Lexer();

    std::vector<Token> tokenize(std::istream * input);

private:

    bool are_from_the_same_token(char a, char b);

    Token get_next_token(std::istream * input);
};
