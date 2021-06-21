#pragma once

#include <variant>

#include "a/s1-lexing/token.hpp"
#include "ast.hpp"


class Token2Ast {
public:

    Token2Ast();

    Token2Ast& operator<<(pToken&& token);

    Token2Ast& operator<<(pAst&& ast_node);

    void operator>>(Token2Ast& receiver);

    pAst extract();

private:

    std::vector<std::variant<pToken, pAst>> stack;
};
