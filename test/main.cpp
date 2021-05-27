#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "a.hpp"
#include <string>


TEST_CASE("Build system check")
{
    CHECK(true);

    SUBCASE("using an exported function")
    {
        CHECK(identity(true) == true);
        CHECK(identity(false) == false);
    }
}
