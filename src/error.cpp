#include "a/prelude.hpp"


const char* CompilerError::what() const throw () {
    return "Compiler Error";
}


const char* UnimplementedError::what() const throw () {
    return "Unimplemented Error";
}


const char* SyntaxError::what() const throw () {
    return "Syntax Error";
}
