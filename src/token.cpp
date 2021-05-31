#include <sstream>

#include "a.hpp"


Token::Token(TokenKind kind) :
    kind(kind)
{}

Token::~Token() = default;


TokenEof::TokenEof() :
    Token(TokenKind::eof)
{}

UpToken TokenEof::make() {
    return std::make_unique<TokenEof>();
}

std::string TokenEof::format() {
    return "eof";
}


TokenBracketLeft::TokenBracketLeft() :
    Token(TokenKind::bracket_left)
{}

UpToken TokenBracketLeft::make() {
    return std::make_unique<TokenBracketLeft>();
}

std::string TokenBracketLeft::format() {
    return "(";
}


TokenBracketRight::TokenBracketRight() :
    Token(TokenKind::bracket_right)
{}

UpToken TokenBracketRight::make() {
    return std::make_unique<TokenBracketRight>();
}

std::string TokenBracketRight::format() {
    return ")";
}


TokenInteger::TokenInteger(int i) :
    Token(TokenKind::integer),
    val(i)
{}

UpToken TokenInteger::make(int i) {
    return std::make_unique<TokenInteger>(i);
}

std::string TokenInteger::format() {
    std::stringstream ss;
    ss << "int:" << val;
    return ss.str();
}

int TokenInteger::value() {
    return val;
}


TokenIdentifier::TokenIdentifier(std::string&& s) :
    Token(TokenKind::identifier),
    val(s)
{}

UpToken TokenIdentifier::make(std::string&& s) {
    return std::make_unique<TokenIdentifier>(std::move(s));
}

std::string TokenIdentifier::format() {
    return "id:" + val;
}

std::string TokenIdentifier::value() {
    return val;
}


TokenMatcher::TokenMatcher() {
    // Fill in the token_matches with lambdas that either fail with
    // nullopt or give you the token. The order of inserting is important,
    // as in every lambda we assume that all previous lambdas have failed
    // and returned nullopt.

    token_matches.emplace_back(
        [](std::string s) -> std::optional<UpToken> {
            if(s != "(") {
                return std::nullopt;
            }
            return {TokenBracketLeft::make()};
        }
    );

    token_matches.emplace_back(
        [](std::string s) -> std::optional<UpToken> {
            if(s != ")") {
                return std::nullopt;
            }
            return {TokenBracketRight::make()};
        }
    );

    token_matches.emplace_back(
        [](std::string s) -> std::optional<UpToken> {
            for(auto it = s.begin(); it < s.end(); it++) {
                if(not std::isdigit(*it)) {
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
        [](std::string s) -> std::optional<UpToken> {
            for(auto it = s.begin(); it < s.end(); it++) {
                if(not isascii(*it) or not std::isgraph(*it)) {
                    return std::nullopt;
                }
            }
            return {TokenIdentifier::make(std::move(s))};
        }
    );
}


UpToken TokenMatcher::match(std::string&& token) {
    // we don't expect any empty strings here -- whitespace should be
    // filtered out, eof should not go here, so getting it would
    // mean there is a bug in the calling code.
    if(token.empty()) {
        throw CompilerError();
    }

    for(auto it : token_matches) {
        std::optional<UpToken> res = it(token);
        if(res != std::nullopt) {
            // Early return to be sure that only one lambda actually
            // produces a value.
            return std::move(*res);
        }
    }

    throw SyntaxError();
}
