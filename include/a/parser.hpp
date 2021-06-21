#pragma once

#include <variant>

#include "ast.hpp"
#include "token.hpp"


class TokenToAst {
public:

    TokenToAst();

    TokenToAst& operator<<(pToken&& token);

    TokenToAst& operator<<(pAst&& ast_node);

    void operator>>(TokenToAst& receiver);

    pAst extract();

private:

    std::vector<std::variant<pToken, pAst>> stack;
};


class Parser {
public:

    Parser();

    pAst parse(std::vector<pToken>&& tokens);

private:

    TokenToAst converter;
};
