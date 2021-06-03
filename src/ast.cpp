#include "a/ast.hpp"


ExprAstNode::ExprAstNode(AstNodeKind kind) :
    kind(kind)
{}

ExprAstNode::~ExprAstNode() = default;


NilAstNode::NilAstNode() :
    ExprAstNode(AstNodeKind::nil)
{}

UpExprAstNode NilAstNode::make() {
    return std::make_unique<NilAstNode>();
}


SeqAstNode::SeqAstNode(std::vector<UpExprAstNode>&& seq) :
    ExprAstNode(AstNodeKind::sequence),
    seq(std::move(seq))
{}

UpExprAstNode SeqAstNode::make(std::vector<UpExprAstNode>&& seq) {
    return std::make_unique<SeqAstNode>(std::move(seq));
}


SpecialFormAstNode::SpecialFormAstNode(std::string&& val) :
    ExprAstNode(AstNodeKind::special_form),
    val(val)
{}

UpExprAstNode SpecialFormAstNode::make(std::string&& val) {
    return std::make_unique<SpecialFormAstNode>(std::move(val));
}


IntegerAstNode::IntegerAstNode(int val) :
    ExprAstNode(AstNodeKind::integer),
    val(val)
{}

UpExprAstNode IntegerAstNode::make(int val) {
    return std::make_unique<IntegerAstNode>(val);
}


IdentifierAstNode::IdentifierAstNode(std::string&& val) :
    ExprAstNode(AstNodeKind::identifier),
    val(val)
{}

UpExprAstNode IdentifierAstNode::make(std::string&& val) {
    return std::make_unique<IdentifierAstNode>(std::move(val));
}
