#include "a/ast.hpp"


ExprAstNode::ExprAstNode() = default;

ExprAstNode::~ExprAstNode() = default;


ExprSeqAstNode::ExprSeqAstNode(std::vector<UpExprAstNode>&& seq) :
    seq(std::move(seq))
{}

UpExprAstNode ExprSeqAstNode::make(std::vector<UpExprAstNode>&& seq) {
    return std::make_unique<ExprSeqAstNode>(std::move(seq));
}


SpecialFormAstNode::SpecialFormAstNode(std::string&& val) :
    val(val)
{}

UpExprAstNode SpecialFormAstNode::make(std::string&& val) {
    return std::make_unique<SpecialFormAstNode>(std::move(val));
}


IntegerAstNode::IntegerAstNode(int val) :
    val(val)
{}

UpExprAstNode IntegerAstNode::make(int val) {
    (void) this->val;
    return std::make_unique<IntegerAstNode>(val);
}


IdentifierAstNode::IdentifierAstNode(std::string&& val) :
    val(val)
{}

UpExprAstNode IdentifierAstNode::make(std::string&& val) {
    return std::make_unique<IdentifierAstNode>(std::move(val));
}
