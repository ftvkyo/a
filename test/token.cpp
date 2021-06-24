#include "prelude.hpp"


TEST_CASE("Token creation and inspection")
{
    std::stringstream ss;
    std::string expected;

    SUBCASE("TokenEof")
    {
        auto tok = TokenEof::make();
        CHECK_EQ(tok->get_kind(), TokenKind::eof);
        tok->inspect(&ss);
        expected = "eof";
    }

    SUBCASE("TokenBrackets")
    {
        auto tok_l = TokenBracketLeft::make();
        CHECK_EQ(tok_l->get_kind(), TokenKind::bracket_left);
        tok_l->inspect(&ss);

        auto tok_r = TokenBracketRight::make();
        CHECK_EQ(tok_r->get_kind(), TokenKind::bracket_right);
        tok_r->inspect(&ss);

        expected = "()";
    }

    SUBCASE("TokenSpecialForm")
    {
        auto tok = TokenSpecialForm::make("@potato");
        CHECK_EQ(tok->get_kind(), TokenKind::special_form);
        tok->inspect(&ss);
        expected = "sf:@potato";
    }

    SUBCASE("TokenInteger")
    {
        auto tok = TokenInteger::make(42);
        CHECK_EQ(tok->get_kind(), TokenKind::integer);
        tok->inspect(&ss);
        expected = "int:42";
    }

    SUBCASE("TokenIdentifier")
    {
        auto tok = TokenIdentifier::make("potato");
        CHECK_EQ(tok->get_kind(), TokenKind::identifier);
        tok->inspect(&ss);
        expected = "id:potato";
    }

    CHECK_EQ(ss.str(), expected);
}


TEST_CASE("Token getters")
{
    SUBCASE("throw when no such data")
    {
        SUBCASE("TokenEof")
        {
            auto tok = TokenEof::make();
            CHECK_THROWS_AS(tok->retrieve_int(), CompilerError);
            CHECK_THROWS_AS(tok->retrieve_symbol(), CompilerError);
        }

        SUBCASE("TokenBrackets")
        {
            auto tok_l = TokenBracketLeft::make();
            CHECK_THROWS_AS(tok_l->retrieve_int(), CompilerError);
            CHECK_THROWS_AS(tok_l->retrieve_symbol(), CompilerError);

            auto tok_r = TokenBracketRight::make();
            CHECK_THROWS_AS(tok_r->retrieve_int(), CompilerError);
            CHECK_THROWS_AS(tok_r->retrieve_symbol(), CompilerError);
        }
    }

    SUBCASE("throw when data is of a different type")
    {
        SUBCASE("TokenSpecialForm")
        {
            auto tok = TokenSpecialForm::make("@potato");
            CHECK_THROWS_AS(tok->retrieve_int(), CompilerError);
        }

        SUBCASE("TokenInteger")
        {
            auto tok = TokenInteger::make(42);
            CHECK_THROWS_AS(tok->retrieve_symbol(), CompilerError);
        }

        SUBCASE("TokenIdentifier")
        {
            auto tok = TokenIdentifier::make("potato");
            CHECK_THROWS_AS(tok->retrieve_int(), CompilerError);
        }
    }

    SUBCASE("work when data is of the correct type")
    {
        SUBCASE("TokenSpecialForm")
        {
            auto tok = TokenSpecialForm::make("@potato");
            CHECK_EQ(tok->retrieve_symbol(), "@potato");
        }

        SUBCASE("TokenInteger")
        {
            auto tok = TokenInteger::make(42);
            CHECK_EQ(tok->retrieve_int(), 42);
        }

        SUBCASE("TokenIdentifier")
        {
            auto tok = TokenIdentifier::make("potato");
            CHECK_EQ(tok->retrieve_symbol(), "potato");
        }
    }
}


TEST_CASE("TokenMatcher")
{
    auto matcher = TokenMatcher();
    std::stringstream input;

    auto f = [](TokenMatcher* matcher, std::stringstream* i) {
        auto tok = matcher->match(i->str());
    };

    SUBCASE("throws on empty")
    {
        CHECK_THROWS_AS(f(&matcher, &input), CompilerError);
    }

    SUBCASE("brackets")
    {
        auto tok_l = matcher.match("(");
        CHECK_EQ(tok_l->get_kind(), TokenKind::bracket_left);

        auto tok_r = matcher.match(")");
        CHECK_EQ(tok_r->get_kind(), TokenKind::bracket_right);
    }

    SUBCASE("special form")
    {
        auto tok = matcher.match("@i-am-special");
        CHECK_EQ(tok->get_kind(), TokenKind::special_form);
        CHECK_EQ(tok->retrieve_symbol(), "@i-am-special");
    }

    SUBCASE("integer")
    {
        auto tok = matcher.match("42");
        CHECK_EQ(tok->get_kind(), TokenKind::integer);
        CHECK_EQ(tok->retrieve_int(), 42);
    }

    SUBCASE("identifier")
    {
        auto tok = matcher.match("identifier?");
        CHECK_EQ(tok->get_kind(), TokenKind::identifier);
        CHECK_EQ(tok->retrieve_symbol(), "identifier?");
    }

    SUBCASE("throws on broken identifiers") {
        SUBCASE("ESC control character")
        {
            input << static_cast<char>(0x1B);
            CHECK_THROWS_AS(f(&matcher, &input), SyntaxError);
        }

        SUBCASE("DEL control character")
        {
            input << static_cast<char>(0x7F);
            CHECK_THROWS_AS(f(&matcher, &input), SyntaxError);
        }

        SUBCASE("CAN control character in the middle of an identifier")
        {
            input << "iden" << static_cast<char>(0x18) << "tifier";
            CHECK_THROWS_AS(f(&matcher, &input), SyntaxError);
        }

        SUBCASE("non-ascii")
        {
            // Russian YO and ZH in UTF-8, null-terminated
            unsigned const char s[] = {0x91, 0xD1, 0xB6, 0xD0, 0x00};
            input << s;
            CHECK_THROWS_AS(f(&matcher, &input), SyntaxError);
        }
    }
}
