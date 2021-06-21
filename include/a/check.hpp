#pragma once

#include "ast.hpp"


class Checker {
public:

    Checker();

    void check(pAst &ast);

private:

    void check_top_level(pAst &ast);

    void check_sequence(pAst &ast);

    void check_special_form_invokation(pAst &ast);
};
