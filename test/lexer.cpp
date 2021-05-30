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
}
