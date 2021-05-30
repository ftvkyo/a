#include "a.hpp"


const char* CompilerError::what() const throw () {
    return "Compiler Error";
}


const char* SyntaxError::what() const throw () {
    return "Syntax Error";
}
