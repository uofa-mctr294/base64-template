#include <gtest/gtest.h>
#include <string>
#include "greet.h"

TEST(MainUnitTest, GreetReturnsHelloWorld)
{
    EXPECT_EQ(greet(), "Greet, World!");
}
