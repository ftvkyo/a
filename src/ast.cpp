#include "a.hpp"


AstExpression::AstExpression(AstKind kind) :
    kind(kind)
{}

std::vector<pAst> AstExpression::get_seq() {
    throw CompilerError();
}

int AstExpression::get_int() {
    throw CompilerError();
}

std::string AstExpression::get_string() {
    throw CompilerError();
}

AstExpression::~AstExpression() = default;


AstSequence::AstSequence(std::vector<pAst>&& seq) :
    AstExpression(AstKind::sequence),
    seq(std::move(seq))
{}

pAst AstSequence::make(std::vector<pAst>&& seq) {
    return pAst(new AstSequence(std::move(seq)));
}

void AstSequence::inspect(std::ostream* out) {
    *out << "(";

    if(seq.size() > 0) {
        seq[0]->inspect(out);
    }

    if(seq.size() > 1) {
        for(size_t i = 1; i < seq.size(); i++) {
            *out << " ";
            seq[i]->inspect(out);
        }
    }

    *out << ")";
}

std::vector<pAst> AstSequence::get_seq() {
    return seq;
}


AstSpecialForm::AstSpecialForm(std::string&& val) :
    AstExpression(AstKind::special_form),
    val(val)
{}

pAst AstSpecialForm::make(std::string&& val) {
    return pAst(new AstSpecialForm(std::move(val)));
}

void AstSpecialForm::inspect(std::ostream* out) {
    *out << val;
}

std::string AstSpecialForm::get_string() {
    return val;
}


AstInteger::AstInteger(int val) :
    AstExpression(AstKind::integer),
    val(val)
{}

pAst AstInteger::make(int val) {
    return pAst(new AstInteger(val));
}

void AstInteger::inspect(std::ostream* out) {
    *out << val;
}

int AstInteger::get_int() {
    return val;
}


AstIdentifier::AstIdentifier(std::string&& val) :
    AstExpression(AstKind::identifier),
    val(val)
{}

pAst AstIdentifier::make(std::string&& val) {
    return pAst(new AstIdentifier(std::move(val)));
}

void AstIdentifier::inspect(std::ostream* out) {
    *out << val;
}

std::string AstIdentifier::get_string() {
    return val;
}
