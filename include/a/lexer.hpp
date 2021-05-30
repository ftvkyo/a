#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <functional>


enum class TokenKind {
    eof,

    bracket_left,
    bracket_right,

    identifier,

    integer,
};


class TokenMatcher {
public:

    TokenMatcher();

    TokenKind match(std::string token);

private:
    typedef std::vector<
        std::pair<
            std::function<bool(std::string)>,
            TokenKind
        >
    > TokenMatches;

    TokenMatches token_matches;

    TokenKind default_token_kind;
};


class Lexer {
public:

    Lexer();

    std::vector<TokenKind> tokenize(std::istream * input);

private:

    bool is_long_token_char(char c);

    bool is_long_token_pair(char left, char right);

    TokenKind get_next_token(std::istream * input);

    std::string SPECIAL_CHARS;

    TokenMatcher token_matcher;
};
