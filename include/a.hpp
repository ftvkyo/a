#pragma once

#include "a/lexer.hpp"


/**
 * There's a bug in the compiler.
 * In other words, throw in situations that should not happen.
 */
class CompilerError : public std::exception {
    const char* what() const throw ();
};


/**
 * Lexer and Parser error.
 */
class SyntaxError : public std::exception {
    const char* what() const throw ();
};
