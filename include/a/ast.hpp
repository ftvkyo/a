#pragma once

#include <vector>
#include <memory>


enum class AstKind {
    sequence,

    special_form,

    integer,

    identifier,
};


class AstExpression {
public:

    AstKind kind;

    virtual void inspect(std::ostream* output) = 0;

    virtual ~AstExpression() = 0;

protected:
    AstExpression(AstKind kind);
};


typedef std::shared_ptr<AstExpression> pAst;


class AstSequence : public AstExpression {
public:
    static pAst make(std::vector<pAst>&& seq);

    virtual void inspect(std::ostream* output);

protected:
    AstSequence(std::vector<pAst>&& seq);

    std::vector<pAst> seq;
};


class AstSpecialForm : public AstExpression {
public:
    static pAst make(std::string&& val);

    virtual void inspect(std::ostream* output);

protected:
    AstSpecialForm(std::string&& val);

    std::string val;
};


class AstInteger : public AstExpression {
public:
    static pAst make(int val);

    virtual void inspect(std::ostream* output);

protected:
    AstInteger(int val);

    int val;
};


class AstIdentifier : public AstExpression {
public:
    static pAst make(std::string&& val);

    virtual void inspect(std::ostream* output);

protected:
    AstIdentifier(std::string&& val);

    std::string val;
};
