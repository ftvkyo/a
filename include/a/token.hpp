#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <optional>


/**
 * Different kinds of tokens we have.
 */
enum class TokenKind {
    /**
     * End of file.
     * Should appear only at the end of file.
     */
    eof,

    /// "("
    bracket_left,
    /// ")"
    bracket_right,

    /// A sequence of characters statring with @
    special_form,

    /// A sequence of characters in range from '0' to '9' of any length.
    integer,

    /**
     * A sequence of graphical ASCII characters of any length that does not
     * fall into any other category.
     */
    identifier,
};


/**
 * Represents a single token.
 * This one is abstract, intended to be used with smart pointers.
 * Actual tokens inherit from this class.
 */
class Token {
public:

    /**
     * Getter for the kind of the token.
     *
     * @returns kind of the token.
     */
    TokenKind get_kind();

    /**
     * Generate a pretty representation of this token.
     *
     * @param out Where to put the pretty thing into.
     */
    virtual void inspect(std::ostream* output) = 0;

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

    virtual ~Token() = 0;

protected:

    /**
     * Hidden constructor to reference from children to set the kind.
     *
     * Each child has a factory method for its creation.
     */
    Token(TokenKind kind);

private:

    /**
     * Kind of this token.
     */
    TokenKind kind;
};


/**
 * For polymorphism, we use Token only as a pointer.
 */
typedef std::shared_ptr<Token> pToken;


struct TokenEof : public Token {
public:
    static pToken make();

    virtual void inspect(std::ostream* output) override;

private:
    TokenEof();
};


struct TokenBracketLeft : public Token {
public:
    static pToken make();

    virtual void inspect(std::ostream* output) override;

private:
    TokenBracketLeft();
};


struct TokenBracketRight : public Token {
public:
    static pToken make();

    virtual void inspect(std::ostream* output) override;

private:
    TokenBracketRight();
};


struct TokenSpecialForm : public Token {
public:
    static pToken make(std::string&& s);

    virtual void inspect(std::ostream* output) override;

    virtual std::string retrieve_symbol() override;

private:
    TokenSpecialForm(std::string&& s);
    std::string val;
};


struct TokenInteger : public Token {
public:
    static pToken make(int i);

    virtual void inspect(std::ostream* out) override;

    virtual int retrieve_int() override;

private:
    TokenInteger(int i);
    int val;
};


struct TokenIdentifier : public Token {
public:
    static pToken make(std::string&& s);

    virtual void inspect(std::ostream* out) override;

    virtual std::string retrieve_symbol() override;

private:
    TokenIdentifier(std::string&& s);
    std::string val;
};


/**
 * Converts std::strings into tokens.
 */
class TokenMatcher {
public:

    TokenMatcher();

    /**
     * Convert a string into a token.
     *
     * @param token String to convert into a token.
     *
     * @returns Result of conversion.
     */
    pToken match(std::string&& token);

private:

    typedef std::vector<
        std::function<
            std::optional<pToken>(std::string)
        >
    > TokenMatches;

    /**
     * List of magical predicates that determine the token kind
     * and functions that convert strings into tokens on success.
     */
    TokenMatches token_matches;
};
