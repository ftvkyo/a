#pragma once

#include <variant>

#include "ast.hpp"
#include "token.hpp"


class TokenToAst {
public:

    enum State {
        GIVE,
        TAKE,
    };

    TokenToAst();

    State state();

    TokenToAst& operator<<(UpToken&& token);

    TokenToAst& operator<<(UpExprAstNode&& ast_node);

    void operator>>(TokenToAst& receiver);

    UpExprAstNode extract();

private:

    std::vector<std::variant<UpToken, UpExprAstNode>> stack;
};


class Parser {
public:

    Parser();

    UpExprAstNode parse(std::vector<UpToken>&& tokens);

private:

    TokenToAst converter;
};
