#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <string>


/**
 * Different kinds of tokes we have.
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


class Token {
public:
    TokenKind kind;
    virtual std::string format() = 0;
    virtual ~Token() = 0;
protected:
    Token(TokenKind kind);
};


struct TokenEof : public Token {
public:
    static std::unique_ptr<Token> make();
    virtual std::string format() override;
private:
    TokenEof();
};


struct TokenBracketLeft : public Token {
public:
    static std::unique_ptr<Token> make();
    virtual std::string format() override;
private:
    TokenBracketLeft();
};


struct TokenBracketRight : public Token {
public:
    static std::unique_ptr<Token> make();
    virtual std::string format() override;
private:
    TokenBracketRight();
};


struct TokenInteger : public Token {
public:
    static std::unique_ptr<Token> make(int i);
    virtual std::string format() override;
    int value();
private:
    TokenInteger(int i);
    int val;
};


struct TokenIdentifier : public Token {
public:
    static std::unique_ptr<Token> make(std::string s);
    virtual std::string format() override;
    std::string value();
private:
    TokenIdentifier(std::string s);
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
    std::unique_ptr<Token> match(std::string token);

private:
    typedef std::vector<
        std::pair<
            std::function<bool(std::string)>,
            std::function<std::unique_ptr<Token>(std::string)>
        >
    > TokenMatches;

    /**
     * List of magical predicates that determine the token kind
     * and functions that convert strings into tokens on success.
     */
    TokenMatches token_matches;
};
