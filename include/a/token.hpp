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
 * Represents a token. This one is abstract, intended to be used
 * with smart pointers. Actual tokens will inherit from this class.
 */
class Token {
public:

    /**
     * Kind of this token.
     */
    TokenKind kind;

    virtual int get_int();

    virtual std::string get_string();

    /**
     * Get a pretty representation of this token.
     *
     * @param out Where to put the pretty thing into.
     */
    virtual void inspect(std::ostream* out) = 0;

    virtual ~Token() = 0;

protected:

    /**
     * Hidden constructor to reference from children to set the kind.
     *
     * Each child has a factory method for its UpToken creation.
     */
    Token(TokenKind kind);
};


/**
 * For the sake of polymorphism, we wrap Token into a shared_ptr.
 * It's just too difficult to type.
 */
typedef std::shared_ptr<Token> pToken;


struct TokenEof : public Token {
public:
    static pToken make();

    virtual void inspect(std::ostream* out) override;

private:
    TokenEof();
};


struct TokenBracketLeft : public Token {
public:
    static pToken make();

    virtual void inspect(std::ostream* out) override;

private:
    TokenBracketLeft();
};


struct TokenBracketRight : public Token {
public:
    static pToken make();

    virtual void inspect(std::ostream* out) override;

private:
    TokenBracketRight();
};


struct TokenSpecialForm : public Token {
public:
    static pToken make(std::string&& s);

    virtual std::string get_string() override;

    virtual void inspect(std::ostream* out) override;

private:
    TokenSpecialForm(std::string&& s);
    std::string val;
};


struct TokenInteger : public Token {
public:
    static pToken make(int i);

    virtual int get_int() override;

    virtual void inspect(std::ostream* out) override;

private:
    TokenInteger(int i);
    int val;
};


struct TokenIdentifier : public Token {
public:
    static pToken make(std::string&& s);

    virtual std::string get_string() override;

    virtual void inspect(std::ostream* out) override;

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
