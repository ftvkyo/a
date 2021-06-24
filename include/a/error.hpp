#pragma once


/**
 * If you see this, there's a bug in the compiler.
 * In other words, throw in situations that should not happen.
 */
class CompilerError : public std::exception {
public:
    CompilerError(std::string what);

    const char* what() const throw ();

private:
    std::string reason;
};


/**
 * If you see this, some part of the compiler hasn't been implemented yet.
 */
class UnimplementedError : public std::exception {
public:
    UnimplementedError(std::string what);

    const char* what() const throw ();

private:
    std::string reason;
};


/**
 * Errors caused by malformed input are represented by this.
 */
class SyntaxError : public std::exception {
public:
    SyntaxError(std::string what);

    const char* what() const throw ();

private:
    std::string reason;
};
