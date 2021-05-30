#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <functional>


/**
 * Different kinds of tokes we have.
 */
enum class TokenKind {
    /**
     * End of file.
     * Should appear only at the end of file.
     */
    eof,

    /// "("
    bracket_left,
    /// ")"
    bracket_right,

    /// A sequence of characters in range from '0' to '9' of any length.
    integer,

    /**
     * A sequence of graphical ASCII characters of any length that does not
     * fall into any other category.
     */
    identifier,
};


/**
 * Converts std::strings into tokens.
 */
class TokenMatcher {
public:

    TokenMatcher();

    /**
     * Convert a string into a token.
     *
     * @param token String to convert into a token.
     *
     * @returns Result of conversion.
     */
    TokenKind match(std::string token);

private:
    typedef std::vector<
        std::pair<
            std::function<bool(std::string)>,
            TokenKind
        >
    > TokenMatches;

    /**
     * List of magical predicates that determine the token kind.
     */
    TokenMatches token_matches;
};


/**
 * Conversion of a meaningless character stream into a vector of tokens.
 */
class Lexer {
public:

    Lexer();

    /**
     * Perform the conversion.
     *
     * @param input The stream to convert to tokens.
     *
     * @returns Result of conversion.
     */
    std::vector<TokenKind> tokenize(std::istream * input);

private:

    /**
     * Some tokens consist of more than one character.
     * This function checks if the given character
     * can be a valid part of such token.
     *
     * @param c The character to check.
     *
     * @returns True if it can be a part of a long token, False otherwise.
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
    TokenKind get_next_token(std::istream * input);

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
