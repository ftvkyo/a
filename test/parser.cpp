#include "prelude.hpp"


TEST_CASE("Parser")
{
    // TODO: test that should throw

    auto lexer = Lexer();
    auto parser = Parser();

    std::stringstream input;
    std::string expected;

    SUBCASE("processing an empty string")
    {
        expected = "()";
    }

    SUBCASE("processing basic brackets")
    {
        input << "()";
        expected = "(())";
    }

    SUBCASE("processing advanced brackets")
    {
        input << "()(()) (()())";
        expected = "(() (()) (() ()))";
    }

    SUBCASE("processing brackets with some data and special forms inbetween")
    {
        input << "(52) (@potat) (identifier?)";
        expected = "((52) (@potat) (identifier?))";
    }

    SUBCASE("processing newlines")
    {
        input << "\n \n";
        expected = "()";
    }

    SUBCASE("just data")
    {
        input << "abc () (15)";
        expected = "(abc () (15))";
    }

    SUBCASE("whitespace")
    {
        input << "abc    ()   \n ( 15      )";
        expected = "(abc () (15))";
    }

    SUBCASE("a little of everything")
    {
        input << "@salad of-potat 256 42.0 (amogus)";
        expected = "(@salad of-potat 256 42.0 (amogus))";
    }

    auto tokens = lexer.tokenize(&input);
    auto node = parser.parse(std::move(tokens));

    std::stringstream output;
    node->inspect(&output);

    CHECK_EQ(output.str(), expected);
}
