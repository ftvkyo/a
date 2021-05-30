#pragma once

#include "a/lexer.hpp"


/**
 * Lexer and Parser error.
 */
class SyntaxError : public std::exception {
    const char* what() const throw ();
};
