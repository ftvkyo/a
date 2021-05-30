#include "prelude.hpp"


TEST_CASE("Lexer")
{
    auto lexer = Lexer();
    std::stringstream input;

    SUBCASE("processing an empty string")
    {
        auto tokens = lexer.tokenize(&input);

        CHECK_EQ(tokens, std::vector<TokenKind>{TokenKind::eof});
    }

    SUBCASE("processing basic brackets")
    {
        input << "()";
        auto tokens = lexer.tokenize(&input);

        std::vector<TokenKind> expected = {
            TokenKind::bracket_left,
            TokenKind::bracket_right,
            TokenKind::eof,
        };

        CHECK_EQ(tokens, expected);
    }

    SUBCASE("processing advanced brackets")
    {
        input << "() (()) )(()";
        auto tokens = lexer.tokenize(&input);

        std::vector<TokenKind> expected = {
            TokenKind::bracket_left,
            TokenKind::bracket_right,

            TokenKind::bracket_left,
            TokenKind::bracket_left,
            TokenKind::bracket_right,
            TokenKind::bracket_right,

            TokenKind::bracket_right,
            TokenKind::bracket_left,
            TokenKind::bracket_left,
            TokenKind::bracket_right,

            TokenKind::eof,
        };

        CHECK_EQ(tokens, expected);
    }

    SUBCASE("processing some brackets and data inbetween")
    {
        input << "(52) (identifier?)";
        auto tokens = lexer.tokenize(&input);

        std::vector<TokenKind> expected = {
            TokenKind::bracket_left,
            TokenKind::integer,
            TokenKind::bracket_right,

            TokenKind::bracket_left,
            TokenKind::identifier,
            TokenKind::bracket_right,

            TokenKind::eof,
        };

        CHECK_EQ(tokens, expected);
    }

    SUBCASE("processing newlines")
    {
        input << "\n \n";
        auto tokens = lexer.tokenize(&input);

        std::vector<TokenKind> expected = {
            TokenKind::eof,
        };

        CHECK_EQ(tokens, expected);
    }

    SUBCASE("throwing on broken identifiers") {
        auto f = [](Lexer * l, std::istream * i){
            auto tokens = l->tokenize(i);
        };

        SUBCASE("ESC control character")
        {
            input << static_cast<char>(0x1B);
            CHECK_THROWS(f(&lexer, &input));
        }

        SUBCASE("DEL control character")
        {
            input << static_cast<char>(0x7F);
            CHECK_THROWS(f(&lexer, &input));
        }

        SUBCASE("CAN control character in the middle of an identifier")
        {
            input << "iden" << static_cast<char>(0x18) << "tifier";
            CHECK_THROWS(f(&lexer, &input));
        }
    }
}
