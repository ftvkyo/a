#pragma once

#include <vector>
#include <memory>


class ExprAstNode {
public:
    virtual ~ExprAstNode() = 0;

protected:
    ExprAstNode();
};


typedef std::unique_ptr<ExprAstNode> UpExprAstNode;


class ExprSeqAstNode : public ExprAstNode {
public:
    UpExprAstNode make(std::vector<UpExprAstNode>&& seq);

private:
    ExprSeqAstNode(std::vector<UpExprAstNode>&& seq);

    std::vector<UpExprAstNode> seq;
};


class IntegerAstNode : public ExprAstNode {
public:
    UpExprAstNode make(int value);

private:
    IntegerAstNode(int value);

    int value;
};


class IdentifierAstNode : public ExprAstNode {
public:
    UpExprAstNode make(std::string value);

private:
    IdentifierAstNode(std::string value);

    std::string value;
};
