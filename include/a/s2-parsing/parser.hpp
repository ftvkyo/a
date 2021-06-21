#pragma once

#include "a/s1-lexing/token.hpp"
#include "a/s1-lexing/token_matcher.hpp"
#include "ast.hpp"
#include "token2ast.hpp"


class Parser {
public:

    Parser();

    pAst parse(std::vector<pToken>&& tokens);

private:

    Token2Ast converter;
};
