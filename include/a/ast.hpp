#pragma once

#include <vector>
#include <memory>


enum class AstKind {
    sequence,

    special_form,

    integer,

    identifier,
};


class AstExpression;


typedef std::shared_ptr<AstExpression> pAst;


class AstExpression {
public:

    AstKind get_kind();

    virtual void inspect(std::ostream* output) = 0;

    virtual std::vector<pAst> retrieve_seq();

    virtual int retrieve_int();

    virtual std::string retrieve_symbol();

    /*
    // Future method for retrieving content of string values
    virtual std::string retrieve_string();
    */

    virtual ~AstExpression() = 0;

protected:

    AstExpression(AstKind kind);

private:

    AstKind kind;
};


class AstSequence : public AstExpression {
public:
    static pAst make(std::vector<pAst>&& seq);

    virtual void inspect(std::ostream* output) override;

    virtual std::vector<pAst> retrieve_seq() override;

protected:
    AstSequence(std::vector<pAst>&& seq);

    std::vector<pAst> seq;
};


class AstSpecialForm : public AstExpression {
public:
    static pAst make(std::string&& val);

    virtual void inspect(std::ostream* output) override;

    virtual std::string retrieve_symbol() override;

protected:
    AstSpecialForm(std::string&& val);

    std::string val;
};


class AstInteger : public AstExpression {
public:
    static pAst make(int val);

    virtual void inspect(std::ostream* output) override;

    virtual int retrieve_int() override;

protected:
    AstInteger(int val);

    int val;
};


class AstIdentifier : public AstExpression {
public:
    static pAst make(std::string&& val);

    virtual void inspect(std::ostream* output) override;

    virtual std::string retrieve_symbol() override;

protected:
    AstIdentifier(std::string&& val);

    std::string val;
};
