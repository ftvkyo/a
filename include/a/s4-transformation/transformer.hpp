#pragma once

#include "a/s2-parsing/ast.hpp"


class Transformer {
public:

    Transformer();

    void transform(pAst ast);

private:

    void transform_define_into_let(pAst ast);
};
