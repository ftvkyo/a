#include "a/ast.hpp"


ExprAstNode::ExprAstNode(AstNodeKind kind) :
    kind(kind)
{}

ExprAstNode::~ExprAstNode() = default;


SeqAstNode::SeqAstNode(std::vector<UpExprAstNode>&& seq) :
    ExprAstNode(AstNodeKind::sequence),
    seq(std::move(seq))
{}

UpExprAstNode SeqAstNode::make(std::vector<UpExprAstNode>&& seq) {
    return std::make_unique<SeqAstNode>(std::move(seq));
}

void SeqAstNode::inspect(std::ostream* out) {
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


SpecialFormAstNode::SpecialFormAstNode(std::string&& val) :
    ExprAstNode(AstNodeKind::special_form),
    val(val)
{}

UpExprAstNode SpecialFormAstNode::make(std::string&& val) {
    return std::make_unique<SpecialFormAstNode>(std::move(val));
}

void SpecialFormAstNode::inspect(std::ostream* out) {
    *out << val;
}


IntegerAstNode::IntegerAstNode(int val) :
    ExprAstNode(AstNodeKind::integer),
    val(val)
{}

UpExprAstNode IntegerAstNode::make(int val) {
    return std::make_unique<IntegerAstNode>(val);
}

void IntegerAstNode::inspect(std::ostream* out) {
    *out << val;
}


IdentifierAstNode::IdentifierAstNode(std::string&& val) :
    ExprAstNode(AstNodeKind::identifier),
    val(val)
{}

UpExprAstNode IdentifierAstNode::make(std::string&& val) {
    return std::make_unique<IdentifierAstNode>(std::move(val));
}

void IdentifierAstNode::inspect(std::ostream* out) {
    *out << val;
}
