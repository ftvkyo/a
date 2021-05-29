#include "prelude.hpp"


TEST_CASE("Lexer")
{
    auto lexer = Lexer();
    std::stringstream input;

    SUBCASE("processing an empty string")
    {
        auto tokens = lexer.tokenize(&input);

        CHECK_EQ(tokens, std::vector<Token>{Token::eof});
    }

    SUBCASE("processing basic brackets")
    {
        input << "()";
        auto tokens = lexer.tokenize(&input);

        std::vector<Token> expected = {
            Token::bracket_left,
            Token::bracket_right,
            Token::eof,
        };

        CHECK_EQ(tokens, expected);
    }

    SUBCASE("processing some brackets and data inbetween")
    {
        input << "(52) (identifier?)";
        auto tokens = lexer.tokenize(&input);

        std::vector<Token> expected = {
            Token::bracket_left,
            Token::number,
            Token::bracket_right,

            Token::identifier,

            Token::eof,
        };

        CHECK_EQ(tokens, expected);
    }
}
