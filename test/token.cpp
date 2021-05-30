#include "prelude.hpp"


TEST_CASE("Token")
{
    SUBCASE("TokenEof")
    {
        auto tok = TokenEof::make();
        CHECK_EQ(tok->kind, TokenKind::eof);
        CHECK_EQ(tok->format(), "eof");
    }

    SUBCASE("TokenBrackets")
    {
        auto tok_l = TokenBracketLeft::make();
        CHECK_EQ(tok_l->kind, TokenKind::bracket_left);
        CHECK_EQ(tok_l->format(), "(");

        auto tok_r = TokenBracketRight::make();
        CHECK_EQ(tok_r->kind, TokenKind::bracket_right);
        CHECK_EQ(tok_r->format(), ")");
    }

    SUBCASE("TokenInteger")
    {
        auto tok = TokenInteger::make(42);
        CHECK_EQ(tok->kind, TokenKind::integer);
        CHECK_EQ(tok->format(), "int:42");
    }

    SUBCASE("TokenIdentifier")
    {
        auto tok = TokenIdentifier::make("potato");
        CHECK_EQ(tok->kind, TokenKind::identifier);
        CHECK_EQ(tok->format(), "id:potato");
    }
}


TEST_CASE("TokenMatcher")
{
    auto matcher = TokenMatcher();
    std::stringstream input;

    auto f = [](TokenMatcher * matcher, std::stringstream * i) {
        auto tok = matcher->match(i->str());
    };

    SUBCASE("throws on empty")
    {
        CHECK_THROWS(f(&matcher, &input));
    }

    SUBCASE("brackets")
    {
        auto tok_l = matcher.match("(");
        CHECK_EQ(tok_l->kind, TokenKind::bracket_left);

        auto tok_r = matcher.match(")");
        CHECK_EQ(tok_r->kind, TokenKind::bracket_right);
    }

    SUBCASE("integer")
    {
        auto tok = matcher.match("42");
        CHECK_EQ(tok->kind, TokenKind::integer);
        CHECK_EQ(dynamic_cast<TokenInteger*>(tok.get())->value(), 42);
    }

    SUBCASE("identifier")
    {
        auto tok = matcher.match("identifier?");
        CHECK_EQ(tok->kind, TokenKind::identifier);
        CHECK_EQ(dynamic_cast<TokenIdentifier*>(tok.get())->value(), "identifier?");
    }

    SUBCASE("throws on broken identifiers") {
        SUBCASE("ESC control character")
        {
            input << static_cast<char>(0x1B);
            CHECK_THROWS(f(&matcher, &input));
        }

        SUBCASE("DEL control character")
        {
            input << static_cast<char>(0x7F);
            CHECK_THROWS(f(&matcher, &input));
        }

        SUBCASE("CAN control character in the middle of an identifier")
        {
            input << "iden" << static_cast<char>(0x18) << "tifier";
            CHECK_THROWS(f(&matcher, &input));
        }

        SUBCASE("non-ascii")
        {
            // Russian YO and ZH in UTF-8, null-terminated
            unsigned const char s[] = {0x91, 0xD1, 0xB6, 0xD0, 0x00};
            input << s;
            CHECK_THROWS(f(&matcher, &input));
        }
    }
}