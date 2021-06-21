#include <sstream>

#include "a/prelude.hpp"


TokenMatcher::TokenMatcher() {
    // Fill in the token_matches with lambdas that either fail with
    // nullopt or give you the token. The order of inserting is important,
    // as in every lambda we assume that all previous lambdas have failed
    // and returned nullopt.

    token_matches.emplace_back(
        [](std::string s) -> std::optional<pToken> {
            if(s != "(") {
                return std::nullopt;
            }
            return {TokenBracketLeft::make()};
        }
    );

    token_matches.emplace_back(
        [](std::string s) -> std::optional<pToken> {
            if(s != ")") {
                return std::nullopt;
            }
            return {TokenBracketRight::make()};
        }
    );

    token_matches.emplace_back(
        [](std::string s) -> std::optional<pToken> {
            if(not s.starts_with("@")) {
                return std::nullopt;
            }
            for(auto c : s) {
                if(not isascii(c) or not std::isgraph(c)) {
                    return std::nullopt;
                }
            }
            return {TokenSpecialForm::make(std::move(s))};
        }
    );

    token_matches.emplace_back(
        [](std::string s) -> std::optional<pToken> {
            for(auto c : s) {
                if(not std::isdigit(c)) {
                    return std::nullopt;
                }
            }

            std::stringstream ss;
            ss << s;
            int result;
            ss >> result;
            return {TokenInteger::make(result)};
        }
    );

    token_matches.emplace_back(
        [](std::string s) -> std::optional<pToken> {
            for(auto c : s) {
                if(not isascii(c) or not std::isgraph(c)) {
                    return std::nullopt;
                }
            }
            return {TokenIdentifier::make(std::move(s))};
        }
    );
}


pToken TokenMatcher::match(std::string&& token) {
    // we don't expect any empty strings here -- whitespace should be
    // filtered out, eof should not go here, so getting it would
    // mean there is a bug in the calling code.
    if(token.empty()) {
        throw CompilerError();
    }

    for(auto it : token_matches) {
        std::optional<pToken> res = it(token);
        if(res != std::nullopt) {
            // Early return to be sure that only one lambda actually
            // produces a value.
            return std::move(*res);
        }
    }

    throw SyntaxError();
}
