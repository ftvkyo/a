#include <sstream>

#include "a.hpp"


Token::Token(TokenKind kind) :
    kind(kind)
{}

Token::~Token() = default;


TokenEof::TokenEof() :
    Token(TokenKind::eof)
{}

std::unique_ptr<Token> TokenEof::make() {
    return std::unique_ptr<TokenEof>(new TokenEof());
}

std::string TokenEof::format() {
    return "eof";
}


TokenBracketLeft::TokenBracketLeft() :
    Token(TokenKind::bracket_left)
{}

std::unique_ptr<Token> TokenBracketLeft::make() {
    return std::unique_ptr<TokenBracketLeft>(new TokenBracketLeft());
}

std::string TokenBracketLeft::format() {
    return "(";
}


TokenBracketRight::TokenBracketRight() :
    Token(TokenKind::bracket_right)
{}

std::unique_ptr<Token> TokenBracketRight::make() {
    return std::unique_ptr<TokenBracketRight>(new TokenBracketRight());
}

std::string TokenBracketRight::format() {
    return ")";
}


TokenInteger::TokenInteger(int i) :
    Token(TokenKind::integer),
    val(i)
{}

std::unique_ptr<Token> TokenInteger::make(int i) {
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

std::unique_ptr<Token> TokenIdentifier::make(std::string s) {
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
        [](std::string s) {
            return s == "(";
        },
        [](std::string s) {
            (void) s;
            return TokenBracketLeft::make();
        }
    );

    token_matches.emplace_back(
        [](std::string s) {
            return s == ")";
        },
        [](std::string s) {
            (void) s;
            return TokenBracketRight::make();
        }
    );

    token_matches.emplace_back(
        [](std::string s) {
            for(auto it = s.begin(); it < s.end(); it++) {
                if(not std::isdigit(*it)) {
                    return false;
                }
            }
            return true;
        },
        [](std::string s) {
            std::stringstream ss;
            ss << s;
            int result;
            ss >> result;
            return TokenInteger::make(result);
        }
    );

    token_matches.emplace_back(
        [](std::string s) {
            for(auto it = s.begin(); it < s.end(); it++) {
                if(not isascii(*it) or not std::isgraph(*it)) {
                    return false;
                }
            }
            return true;
        },
        [](std::string s) {
            return TokenIdentifier::make(s);
        }
    );
}


std::unique_ptr<Token> TokenMatcher::match(std::string token) {
    if(token.empty()) {
        throw CompilerError();
    }

    for(auto it = token_matches.begin(); it < token_matches.end(); it++) {
        if(it->first(token)) {
            return it->second(token);
        }
    }

    throw SyntaxError();
}
