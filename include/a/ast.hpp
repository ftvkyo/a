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

    friend std::unique_ptr<ExprSeqAstNode> std::make_unique<ExprSeqAstNode>(std::vector<UpExprAstNode>&&);

private:
    ExprSeqAstNode(std::vector<UpExprAstNode>&& seq);

    std::vector<UpExprAstNode> seq;
};


class SpecialFormAstNode : public ExprAstNode {
public:
    UpExprAstNode make(std::string&& val);

    friend std::unique_ptr<SpecialFormAstNode> std::make_unique<SpecialFormAstNode>(std::string&&);

private:
    SpecialFormAstNode(std::string&& val);

    std::string val;
};


class IntegerAstNode : public ExprAstNode {
public:
    UpExprAstNode make(int val);

    friend std::unique_ptr<IntegerAstNode> std::make_unique<IntegerAstNode>(int&);

private:
    IntegerAstNode(int val);

    int val;
};


class IdentifierAstNode : public ExprAstNode {
public:
    UpExprAstNode make(std::string&& val);

    friend std::unique_ptr<IdentifierAstNode> std::make_unique<IdentifierAstNode>(std::string&&);

private:
    IdentifierAstNode(std::string&& val);

    std::string val;
};
