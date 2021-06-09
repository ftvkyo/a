#include <sstream>

#include "a.hpp"


Token::Token(TokenKind kind) :
    kind(kind)
{}

int Token::get_int() {
    throw CompilerError();
}

std::string Token::get_string() {
    throw CompilerError();
}

Token::~Token() = default;


TokenEof::TokenEof() :
    Token(TokenKind::eof)
{}

UpToken TokenEof::make() {
    return std::make_unique<TokenEof>();
}

void TokenEof::inspect(std::ostream* out) {
    *out << "eof";
}


TokenBracketLeft::TokenBracketLeft() :
    Token(TokenKind::bracket_left)
{}

UpToken TokenBracketLeft::make() {
    return std::make_unique<TokenBracketLeft>();
}

void TokenBracketLeft::inspect(std::ostream* out) {
    *out << "(";
}


TokenBracketRight::TokenBracketRight() :
    Token(TokenKind::bracket_right)
{}

UpToken TokenBracketRight::make() {
    return std::make_unique<TokenBracketRight>();
}

void TokenBracketRight::inspect(std::ostream* out) {
    *out << ")";
}


TokenSpecialForm::TokenSpecialForm(std::string&& s) :
    Token(TokenKind::special_form),
    val(s)
{}

UpToken TokenSpecialForm::make(std::string&& s) {
    return std::make_unique<TokenSpecialForm>(std::move(s));
}

std::string TokenSpecialForm::get_string() {
    return val;
}

void TokenSpecialForm::inspect(std::ostream* out) {
    *out << "sf:" << val;
}


TokenInteger::TokenInteger(int i) :
    Token(TokenKind::integer),
    val(i)
{}

UpToken TokenInteger::make(int i) {
    return std::make_unique<TokenInteger>(i);
}

int TokenInteger::get_int() {
    return val;
}

void TokenInteger::inspect(std::ostream* out) {
    *out << "int:" << val;
}


TokenIdentifier::TokenIdentifier(std::string&& s) :
    Token(TokenKind::identifier),
    val(s)
{}

UpToken TokenIdentifier::make(std::string&& s) {
    return std::make_unique<TokenIdentifier>(std::move(s));
}

std::string TokenIdentifier::get_string() {
    return val;
}

void TokenIdentifier::inspect(std::ostream* out) {
    *out << "id:" << val;
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
        [](std::string s) -> std::optional<UpToken> {
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
        [](std::string s) -> std::optional<UpToken> {
            for(auto c : s) {
                if(not isascii(c) or not std::isgraph(c)) {
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
