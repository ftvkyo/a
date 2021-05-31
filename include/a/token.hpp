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

    /**
     * Get a pretty representation of this token.
     *
     * @returns Prettily formatted token, for printing.
     */
    virtual std::string format() = 0;

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
 * For the sake of polymorphism, we wrap Token into a unique_ptr.
 * It's just too difficult to type.
 */
typedef std::unique_ptr<Token> UpToken;


struct TokenEof : public Token {
public:
    static UpToken make();
    virtual std::string format() override;

    friend std::unique_ptr<TokenEof> std::make_unique<TokenEof>();

private:
    TokenEof();
};


struct TokenBracketLeft : public Token {
public:
    static UpToken make();
    virtual std::string format() override;

    friend std::unique_ptr<TokenBracketLeft> std::make_unique<TokenBracketLeft>();

private:
    TokenBracketLeft();
};


struct TokenBracketRight : public Token {
public:
    static UpToken make();
    virtual std::string format() override;

    friend std::unique_ptr<TokenBracketRight> std::make_unique<TokenBracketRight>();

private:
    TokenBracketRight();
};


struct TokenInteger : public Token {
public:
    static UpToken make(int i);
    virtual std::string format() override;

    /**
     * Get the value of the token.
     *
     * @returns The original integer value of the token.
     */
    int value();

    friend std::unique_ptr<TokenInteger> std::make_unique<TokenInteger>(int&);

private:
    TokenInteger(int i);
    int val;
};


struct TokenIdentifier : public Token {
public:
    static UpToken make(std::string&& s);
    virtual std::string format() override;

    /**
     * Get the value of the token.
     *
     * @returns The original std::string value of the token.
     */
    std::string value();

    friend std::unique_ptr<TokenIdentifier> std::make_unique<TokenIdentifier>(std::string&&);

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
    UpToken match(std::string&& token);

private:

    typedef std::vector<
        std::function<
            std::optional<UpToken>(std::string)
        >
    > TokenMatches;

    /**
     * List of magical predicates that determine the token kind
     * and functions that convert strings into tokens on success.
     */
    TokenMatches token_matches;
};
