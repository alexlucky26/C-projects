#include "lib.h"
#include <gtest/gtest.h>

TEST(VersionTest, ValidVersion) {
    EXPECT_GT(version(), 0);
}

// Главная функция GoogleTest
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}