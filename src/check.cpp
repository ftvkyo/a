#include "a.hpp"


Checker::Checker() = default;

void Checker::check(pAst &ast) {
    check_top_level(ast);
    check_sequence(ast);
}

void Checker::check_top_level(pAst &ast) {
    bool isSeq = ast->kind == AstKind::sequence;
    if(!isSeq) {
        throw CompilerError();
    }

    auto seq = ast->get_seq();
    bool isBlock = seq.size() != 0
        && seq[0]->kind == AstKind::special_form
        && seq[0]->get_string() == "@block";
    if(!isBlock) {
        throw CompilerError();
    }

    check_sequence(ast);
}

void Checker::check_sequence(pAst &ast) {
    (void) ast;
}

void Checker::check_special_form_invokation(pAst &ast) {
    (void) ast;
}
