#include <sstream>

#include "a.hpp"


Token::Token(TokenKind kind) :
    kind(kind)
{}

Token::~Token() = default;


TokenEof::TokenEof() :
    Token(TokenKind::eof)
{}

UPToken TokenEof::make() {
    return std::unique_ptr<TokenEof>(new TokenEof());
}

std::string TokenEof::format() {
    return "eof";
}


TokenBracketLeft::TokenBracketLeft() :
    Token(TokenKind::bracket_left)
{}

UPToken TokenBracketLeft::make() {
    return std::unique_ptr<TokenBracketLeft>(new TokenBracketLeft());
}

std::string TokenBracketLeft::format() {
    return "(";
}


TokenBracketRight::TokenBracketRight() :
    Token(TokenKind::bracket_right)
{}

UPToken TokenBracketRight::make() {
    return std::unique_ptr<TokenBracketRight>(new TokenBracketRight());
}

std::string TokenBracketRight::format() {
    return ")";
}


TokenInteger::TokenInteger(int i) :
    Token(TokenKind::integer),
    val(i)
{}

UPToken TokenInteger::make(int i) {
    return std::unique_ptr<TokenInteger>(new TokenInteger(i));
}

std::string TokenInteger::format() {
    std::stringstream ss;
    ss << "int:" << val;
    return ss.str();
}

int TokenInteger::value() {
    return val;
}


TokenIdentifier::TokenIdentifier(std::string s) :
    Token(TokenKind::identifier),
    val(s)
{}

UPToken TokenIdentifier::make(std::string s) {
    return std::unique_ptr<TokenIdentifier>(new TokenIdentifier(s));
}

std::string TokenIdentifier::format() {
    return "id:" + val;
}

std::string TokenIdentifier::value() {
    return val;
}


TokenMatcher::TokenMatcher() {
    token_matches.emplace_back(
        [](std::string s) -> std::optional<UPToken> {
            if(s != "(") {
                return std::nullopt;
            }
            return {TokenBracketLeft::make()};
        }
    );

    token_matches.emplace_back(
        [](std::string s) -> std::optional<UPToken> {
            if(s != ")") {
                return std::nullopt;
            }
            return {TokenBracketRight::make()};
        }
    );

    token_matches.emplace_back(
        [](std::string s) -> std::optional<UPToken> {
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
        [](std::string s) -> std::optional<UPToken> {
            for(auto it = s.begin(); it < s.end(); it++) {
                if(not isascii(*it) or not std::isgraph(*it)) {
                    return std::nullopt;
                }
            }
            return {TokenIdentifier::make(s)};
        }
    );
}


UPToken TokenMatcher::match(std::string token) {
    if(token.empty()) {
        throw CompilerError();
    }

    for(auto it = token_matches.begin(); it < token_matches.end(); it++) {
        std::optional<UPToken> res = (*it)(token);
        if(res != std::nullopt) {
            return std::move(*res);
        }
    }

    throw SyntaxError();
}
