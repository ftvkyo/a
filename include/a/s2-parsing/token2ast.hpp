#pragma once

#include <variant>

#include "a/s1-lexing/token.hpp"
#include "ast.hpp"


/**
 * Convert a series of same depth level pToken-s and pAst-s into an AstSequence.
 */
class Token2Ast {
public:

    Token2Ast();

    /**
     * Add a token to the series.
     *
     * @param token The token to add.
     *
     * @returns itself.
     */
    Token2Ast& operator<<(pToken token);

    /**
     * Add an AST node to the series.
     *
     * @param ast_node The AST node to add.
     *
     * @returns itself.
     */
    Token2Ast& operator<<(pAst ast_node);

    /**
     * Move the current series stored in `this` into the `reciever` as
     * one AstSequence.
     * Clear the stored series.
     *
     * @param receiver Where to add the AstSequence made of the stored series.
     */
    void operator>>(Token2Ast& receiver);

    /**
     * Extract the current series stored in `this` as an AstSequence.
     * Clear the stored series.
     *
     * @returns AstSequence containing what was in the current series.
     */
    pAst extract();

    /**
     * Get the size of the internal stack.
     *
     * @returns stack.size()
     */
    size_t size();

private:

    std::vector<std::variant<pToken, pAst>> stack;
};
