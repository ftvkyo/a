#include <sstream>

#include "a/prelude.hpp"


Token::Token(TokenKind kind) :
    kind(kind)
{}

TokenKind Token::get_kind() {
    return kind;
}

int Token::retrieve_int() {
    throw CompilerError("Tried to get a stored int from a wrong token.");
}

std::string Token::retrieve_symbol() {
    throw CompilerError("Tried to get a stored symbol from a wrong token.");
}

Token::~Token() = default;


TokenEof::TokenEof() :
    Token(TokenKind::eof)
{}

pToken TokenEof::make() {
    return pToken(new TokenEof());
}

void TokenEof::inspect(std::ostream* out) {
    *out << "eof";
}


TokenBracketLeft::TokenBracketLeft() :
    Token(TokenKind::bracket_left)
{}

pToken TokenBracketLeft::make() {
    return pToken(new TokenBracketLeft());
}

void TokenBracketLeft::inspect(std::ostream* out) {
    *out << "(";
}


TokenBracketRight::TokenBracketRight() :
    Token(TokenKind::bracket_right)
{}

pToken TokenBracketRight::make() {
    return pToken(new TokenBracketRight());
}

void TokenBracketRight::inspect(std::ostream* out) {
    *out << ")";
}


TokenSpecialForm::TokenSpecialForm(std::string&& s) :
    Token(TokenKind::special_form),
    val(s)
{}

pToken TokenSpecialForm::make(std::string&& s) {
    return pToken(new TokenSpecialForm(std::move(s)));
}

void TokenSpecialForm::inspect(std::ostream* out) {
    *out << "sf:" << val;
}

std::string TokenSpecialForm::retrieve_symbol() {
    return val;
}


TokenInteger::TokenInteger(int i) :
    Token(TokenKind::integer),
    val(i)
{}

pToken TokenInteger::make(int i) {
    return pToken(new TokenInteger(i));
}

void TokenInteger::inspect(std::ostream* out) {
    *out << "int:" << val;
}

int TokenInteger::retrieve_int() {
    return val;
}


TokenIdentifier::TokenIdentifier(std::string&& s) :
    Token(TokenKind::identifier),
    val(s)
{}

pToken TokenIdentifier::make(std::string&& s) {
    return pToken(new TokenIdentifier(std::move(s)));
}

void TokenIdentifier::inspect(std::ostream* out) {
    *out << "id:" << val;
}

std::string TokenIdentifier::retrieve_symbol() {
    return val;
}
