#include "a/prelude.hpp"


Checker::Checker() = default;

void Checker::check(pAst ast) {
    check_top_level(ast);
    check_sequence(ast);
}

void Checker::check_top_level(pAst ast) {
    bool isSeq = ast->get_kind() == AstKind::sequence;
    if(!isSeq) {
        throw CompilerError("Top-level expression is not a sequence.");
    }

    auto seq = ast->retrieve_seq();
    bool isBlock = seq.size() != 0
        && seq[0]->get_kind() == AstKind::special_form
        && seq[0]->retrieve_symbol() == "@block";
    if(!isBlock) {
        throw CompilerError("Top-level expression is not a @block special form.");
    }

    check_sequence(ast);
}

void Checker::check_sequence(pAst ast) {
    (void) ast;
}

void Checker::check_special_form_invokation(pAst ast) {
    (void) ast;
}
