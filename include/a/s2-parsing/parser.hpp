#pragma once

#include "a/s1-lexing/token.hpp"
#include "a/s1-lexing/token_matcher.hpp"
#include "ast.hpp"
#include "token2ast.hpp"


/**
 * Parses tokens into an AST.
 */
class Parser {
public:

    Parser();

    /**
     * Parse tokens and return the AST if it was successful.
     *
     * @throws If tokens can't be parsed.
     *
     * @param tokens Tokens to parse.
     *
     * @returns AST generated from the tokens.
     */
    pAst parse(std::vector<pToken>&& tokens);

private:

    Token2Ast converter;
};
