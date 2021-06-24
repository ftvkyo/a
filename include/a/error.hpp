#pragma once


/**
 * If you see this, there's a bug in the compiler.
 * In other words, throw in situations that should not happen.
 */
class CompilerError : public std::exception {
    const char* what() const throw ();
};


/**
 * If you see this, some part of the compiler hasn't been implemented yet.
 */
class UnimplementedError : public std::exception {
    const char* what() const throw ();
};


/**
 * Errors caused by malformed input are represented by this.
 */
class SyntaxError : public std::exception {
    const char* what() const throw ();
};
