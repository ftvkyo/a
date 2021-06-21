#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "token.hpp"


/**
 * Conversion of a meaningless character stream into a vector of tokens.
 */
class Lexer {
public:

    Lexer();

    /**
     * Perform the conversion.
     *
     * @param input The stream to convert into tokens.
     *
     * @returns Result of conversion.
     */
    std::vector<pToken> tokenize(std::istream * input);

private:

    /**
     * Some tokens consist of more than one character.
     * This function checks if the given character
     * can be a valid part of such token.
     *
     * @param c The character to check.
     *
     * @returns true if it can be a part of a long token, false otherwise.
     */
    bool is_long_token_char(char c);

    /**
     * Checks if the given characters can be next to each other in a token.
     *
     * Helps finding where one token ends and another token starts.
     * Both characters can safely be non-token characters - this would mean
     * they can't be next to each other in a token, which is true.
     *
     * @param left The first character.
     * @param right The character next to it.
     *
     * @returns True if left and right could be next to each other in a token.
     */
    bool is_long_token_pair(char left, char right);

    /**
     * Extracts the next token from the given stream.
     *
     * @param input The stream to extract characters from.
     *
     * @returns The token that was extracted.
     */
    pToken get_next_token(std::istream * input);

    /**
     * List of characters that are tokens on their own even if they could
     * technically be a part of a long token.
     */
    std::string SPECIAL_CHARS;

    /**
     * A helper to convert strings to tokens.
     */
    TokenMatcher token_matcher;
};
