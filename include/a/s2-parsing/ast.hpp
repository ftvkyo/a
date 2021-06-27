#pragma once

#include <vector>
#include <memory>


/**
 * Different kinds of AST nodes we have.
 */
enum class AstKind {
    /**
     * A node with children that is a function call.
     */
    function,

    /**
     * A node with children that is a special form "call".
     */
    special_form,

    /**
     * A leaf node that is an integer value.
     */
    integer,

    /**
     * A leaf node that is a name of a special form.
     */
    keyword,

    /**
     * A leaf node that is an identifier.
     */
    identifier,
};


class AstExpression;


/**
 * For polymorphism, we use AstExpression only as a pointer.
 */
typedef std::shared_ptr<AstExpression> pAst;


/**
 * Represents a single AST node potentially with subnodes.
 * This one is abstract, intended to be used with smart pointers.
 * Actual AST nodes inherit from this class.
 */
class AstExpression {
public:

    /**
     * Getter for the kind of the AST node.
     *
     * @returns kind of the node.
     */
    AstKind get_kind();

    /**
     * Generate a pretty representation of this AST node
     * including potential subnodes.
     *
     * @param output Where to put the pretty thing into.
     */
    virtual void inspect(std::ostream* output) = 0;

    /**
     * Get the stored sequence of AST nodes if it exists in the object.
     * Throw otherwise.
     *
     * @throws When there's no sequence in the object.
     *
     * @returns Stored sequence of AST nodes.
     */
    virtual std::vector<pAst> retrieve_seq();

    /**
     * Get the size of the stored sequence of AST nodes if it exists
     * in the object.
     *
     * @throws When there is no sequence in the object.
     *
     * @returns Size of the stored sequence of AST nodes.
     */
    virtual size_t retrieve_seq_s();

    /**
     * Get the i-th element of the stored sequence of AST nodes
     * if it exists in the object.
     *
     * @throws When there is no sequence or the sequence is not long enough.
     *
     * @returns i-th element of the stored sequence of AST nodes.
     */
    virtual pAst retrieve_seq_i(size_t i);

    /**
     * Get the stored integer value if it exists in the object.
     * Throw otherwise.
     *
     * @throws When there's no int in the object.
     *
     * @returns Stored integer.
     */
    virtual int retrieve_int();

    /**
     * Get the stored symbol if it exists in the object.
     * Throw otherwise.
     *
     * @note symbol is, for example, a name of an identifier or a special form.
     *
     * @throws When there's no symbol in the object.
     *
     * @returns Stored symbol.
     */
    virtual std::string retrieve_symbol();

    /*
    // Future method for retrieving content of string values
    virtual std::string retrieve_string();
    */

    virtual ~AstExpression() = 0;

protected:

    /**
     * Hidden constructor to reference from children to set the kind.
     *
     * Each child has a factory method for its creation.
     */
    AstExpression(AstKind kind);

private:

    /**
     * Kind of this AST node.
     */
    AstKind kind;
};


class AstCall : public AstExpression {
public:
    virtual void inspect(std::ostream* output) override;

    virtual std::vector<pAst> retrieve_seq() override;

    virtual size_t retrieve_seq_s() override;

    virtual pAst retrieve_seq_i(size_t i) override;

    virtual ~AstCall() = 0;

protected:
    AstCall(AstKind t, std::vector<pAst>&& seq);

    std::vector<pAst> seq;
};


class AstFunction : public AstCall {
public:
    static pAst make(std::vector<pAst>&& seq);

protected:
    AstFunction(std::vector<pAst>&& seq);
};


class AstSpecialForm : public AstCall {
public:
    static pAst make(std::vector<pAst>&& seq);

protected:
    AstSpecialForm(std::vector<pAst>&& seq);
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


class AstKeyword : public AstExpression {
public:
    static pAst make(std::string&& val);

    virtual void inspect(std::ostream* output) override;

    virtual std::string retrieve_symbol() override;

protected:
    AstKeyword(std::string&& val);

    std::string val;
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
