#pragma once

#include "a/lexer.hpp"
#include "a/parser.hpp"
#include "a/convert.hpp"
#include "a/check.hpp"
#include "a/emit.hpp"


/**
 * If you see it, there's a bug in the compiler.
 * In other words, throw in situations that should not happen.
 */
class CompilerError : public std::exception {
    const char* what() const throw ();
};


/**
 * Lexer and Parser errors are represented by this.
 */
class SyntaxError : public std::exception {
    const char* what() const throw ();
};
