#include "a/prelude.hpp"


CompilerError::CompilerError(std::string what) {
    reason = "Compiler Error: " + what;
}

const char* CompilerError::what() const throw () {
    return reason.c_str();
}


UnimplementedError::UnimplementedError(std::string what) {
    reason = "Unimplemented Error: " + what;
}

const char* UnimplementedError::what() const throw () {
    return reason.c_str();
}


SyntaxError::SyntaxError(std::string what) {
    reason = "Syntax Error: " + what;
}

const char* SyntaxError::what() const throw () {
    return reason.c_str();
}
