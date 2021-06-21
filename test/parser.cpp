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
        expected = "(@block)";
    }

    SUBCASE("processing basic brackets")
    {
        input << "()";
        expected = "(@block ())";
    }

    SUBCASE("processing advanced brackets")
    {
        input << "()(()) (()())";
        expected = "(@block () (()) (() ()))";
    }

    SUBCASE("processing brackets with some data and special forms inbetween")
    {
        input << "(52) (@potat) (identifier?)";
        expected = "(@block (52) (@potat) (identifier?))";
    }

    SUBCASE("processing newlines")
    {
        input << "\n \n";
        expected = "(@block)";
    }

    SUBCASE("just data")
    {
        input << "abc () (15)";
        expected = "(@block abc () (15))";
    }

    SUBCASE("whitespace")
    {
        input << "abc    ()   \n ( 15      )";
        expected = "(@block abc () (15))";
    }

    SUBCASE("a little of everything")
    {
        input << "@salad of-potat 256 42.0 (amogus)";
        expected = "(@block @salad of-potat 256 42.0 (amogus))";
    }

    auto tokens = lexer.tokenize(&input);
    auto node = parser.parse(std::move(tokens));

    std::stringstream output;
    node->inspect(&output);

    CHECK_EQ(output.str(), expected);
}
