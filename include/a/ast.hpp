#pragma once

#include <vector>
#include <memory>


enum class AstNodeKind {
    // ()
    nil,

    sequence,

    special_form,

    integer,

    identifier,
};


class ExprAstNode {
public:
    virtual ~ExprAstNode() = 0;

    AstNodeKind kind;

protected:
    ExprAstNode(AstNodeKind kind);
};


typedef std::unique_ptr<ExprAstNode> UpExprAstNode;


class NilAstNode : public ExprAstNode {
public:
    static UpExprAstNode make();

    friend std::unique_ptr<NilAstNode> std::make_unique<NilAstNode>();

protected:
    NilAstNode();
};


class SeqAstNode : public ExprAstNode {
public:
    static UpExprAstNode make(std::vector<UpExprAstNode>&& seq);

    friend std::unique_ptr<SeqAstNode> std::make_unique<SeqAstNode>(std::vector<UpExprAstNode>&&);

protected:
    SeqAstNode(std::vector<UpExprAstNode>&& seq);

    std::vector<UpExprAstNode> seq;
};


class SpecialFormAstNode : public ExprAstNode {
public:
    static UpExprAstNode make(std::string&& val);

    friend std::unique_ptr<SpecialFormAstNode> std::make_unique<SpecialFormAstNode>(std::string&&);

protected:
    SpecialFormAstNode(std::string&& val);

    std::string val;
};


class IntegerAstNode : public ExprAstNode {
public:
    static UpExprAstNode make(int val);

    friend std::unique_ptr<IntegerAstNode> std::make_unique<IntegerAstNode>(int&);

protected:
    IntegerAstNode(int val);

    int val;
};


class IdentifierAstNode : public ExprAstNode {
public:
    static UpExprAstNode make(std::string&& val);

    friend std::unique_ptr<IdentifierAstNode> std::make_unique<IdentifierAstNode>(std::string&&);

protected:
    IdentifierAstNode(std::string&& val);

    std::string val;
};
