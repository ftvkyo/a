#pragma once

#include "token.hpp"


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
    pToken match(std::string&& token);

private:

    typedef std::vector<
        std::function<
            std::optional<pToken>(std::string)
        >
    > TokenMatches;

    /**
     * List of magical predicates that determine the token kind
     * and functions that convert strings into tokens on success.
     */
    TokenMatches token_matches;
};
