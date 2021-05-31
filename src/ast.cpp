#include "a/ast.hpp"


ExprAstNode::ExprAstNode() = default;

ExprAstNode::~ExprAstNode() = default;


ExprSeqAstNode::ExprSeqAstNode(std::vector<UpExprAstNode>&& seq) :
    seq(std::move(seq))
{}

UpExprAstNode ExprSeqAstNode::make(std::vector<UpExprAstNode>&& seq) {
    return std::unique_ptr<ExprAstNode>(new ExprSeqAstNode(std::move(seq)));
}


IntegerAstNode::IntegerAstNode(int value) :
    value(value)
{}

UpExprAstNode IntegerAstNode::make(int value) {
    return std::unique_ptr<ExprAstNode>(new IntegerAstNode(value));
}


IdentifierAstNode::IdentifierAstNode(std::string value) :
    value(value)
{}

UpExprAstNode IdentifierAstNode::make(std::string value) {
    return std::unique_ptr<ExprAstNode>(new IdentifierAstNode(value));
}
