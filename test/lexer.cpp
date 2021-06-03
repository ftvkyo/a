#include "prelude.hpp"


TEST_CASE("Lexer")
{
    auto lexer = Lexer();
    std::stringstream input;
    std::vector<TokenKind> expected;

    SUBCASE("processing an empty string")
    {
        expected = {
            TokenKind::eof,
        };
    }

    SUBCASE("processing basic brackets")
    {
        input << "()";
        expected = {
            TokenKind::bracket_left,
            TokenKind::bracket_right,
            TokenKind::eof,
        };
    }

    SUBCASE("processing advanced brackets")
    {
        input << "() (()) )(()";
        expected = {
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
    }

    SUBCASE("processing brackets with some data and special forms inbetween")
    {
        input << "(52) @potat (identifier?)";
        expected = {
            TokenKind::bracket_left,
            TokenKind::integer,
            TokenKind::bracket_right,

            TokenKind::special_form,

            TokenKind::bracket_left,
            TokenKind::identifier,
            TokenKind::bracket_right,

            TokenKind::eof,
        };
    }

    SUBCASE("processing newlines")
    {
        input << "\n \n";
        expected = {
            TokenKind::eof,
        };
    }

    auto tokens = lexer.tokenize(&input);

    CHECK_EQ(tokens.size(), expected.size());
    for(size_t i = 0; i < tokens.size(); i++) {
        CHECK_EQ(tokens[i]->kind, expected[i]);
    }
}


TEST_CASE("Lexer pretty-print")
{
    auto lexer = Lexer();
    std::stringstream input;
    std::stringstream output;
    std::string expected;

    SUBCASE("simple")
    {
        input << "abc () ( 15 )";
        expected = "id:abc ( ) ( int:15 ) eof ";
    }

    SUBCASE("whitespace")
    {
        input << "abc    ()   \n ( 15      )";
        expected = "id:abc ( ) ( int:15 ) eof ";
    }

    SUBCASE("identifiers and integers touch brackets")
    {
        input << "abc() (15) a2 5c 1.2 )what?)";
        expected = "id:abc ( ) ( int:15 ) id:a2 id:5c id:1.2 ) id:what? ) eof ";
    }

    SUBCASE("a little of everything")
    {
        input << "@salad of-potat 256 42.0 (amogus)";
        expected = "sf:@salad id:of-potat int:256 id:42.0 ( id:amogus ) eof ";
    }

    auto tokens = lexer.tokenize(&input);
    for(auto it = tokens.begin(); it < tokens.end(); it++) {
        output << (*it)->format() << " ";
    }

    CHECK_EQ(output.str(), expected);
}
