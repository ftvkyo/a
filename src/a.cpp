#include "a.hpp"


const char* SyntaxError::what() const throw () {
    return "Syntax Error";
}
