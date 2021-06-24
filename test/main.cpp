#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "prelude.hpp"


TEST_CASE("Build system check")
{
    CHECK(true);

    SUBCASE("using exported symbols")
    {
        CHECK_THROWS(throw SyntaxError("test"));
    }
}
