#include "a/prelude.hpp"


AstExpression::AstExpression(AstKind kind) :
    kind(kind)
{}

AstKind AstExpression::get_kind() {
    return kind;
}

std::vector<pAst> AstExpression::retrieve_seq() {
    throw CompilerError("Tried to get a sequence from a wrong ast node.");
}

size_t AstExpression::retrieve_seq_s() {
    throw CompilerError("Tried to get a sequence size from a wrong ast node.");
}

pAst AstExpression::retrieve_seq_i(size_t i) {
    (void) i;
    throw CompilerError("Tried to get a sequence element from a wrong ast node.");
}

int AstExpression::retrieve_int() {
    throw CompilerError("Tried to get an int from a wrong ast node.");
}

std::string AstExpression::retrieve_symbol() {
    throw CompilerError("Tried to get a symbol from a wrong ast node.");
}

AstExpression::~AstExpression() = default;


AstCall::AstCall(AstKind t, std::vector<pAst>&& seq) :
    AstExpression(t),
    seq(seq)
{}

void AstCall::inspect(std::ostream* out) {
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

std::vector<pAst> AstCall::retrieve_seq() {
    return seq;
}

size_t AstCall::retrieve_seq_s() {
    return seq.size();
}

pAst AstCall::retrieve_seq_i(size_t i) {
    if(i >= seq.size()) {
        throw CompilerError("Tried to access a non-existent sequence position.");
    }
    return seq[i];
}

AstCall::~AstCall() = default;


AstFunction::AstFunction(std::vector<pAst>&& seq) :
    AstCall(AstKind::function, std::move(seq))
{}

pAst AstFunction::make(std::vector<pAst>&& seq) {
    return pAst(new AstFunction(std::move(seq)));
}


AstSpecialForm::AstSpecialForm(std::vector<pAst>&& seq) :
    AstCall(AstKind::special_form, std::move(seq))
{}

pAst AstSpecialForm::make(std::vector<pAst>&& seq) {
    return pAst(new AstSpecialForm(std::move(seq)));
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

int AstInteger::retrieve_int() {
    return val;
}


AstKeyword::AstKeyword(std::string&& val) :
    AstExpression(AstKind::keyword),
    val(val)
{}

pAst AstKeyword::make(std::string&& val) {
    return pAst(new AstKeyword(std::move(val)));
}

void AstKeyword::inspect(std::ostream* out) {
    *out << val;
}

std::string AstKeyword::retrieve_symbol() {
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

std::string AstIdentifier::retrieve_symbol() {
    return val;
}
