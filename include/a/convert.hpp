#pragma once

#include "ast.hpp"


class Converter {
public:

    Converter();

    void convert(pAst &ast);

private:

    void convert_define_into_let(pAst &ast);
};
