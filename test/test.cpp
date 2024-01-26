#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

#include "figureImages.h"
#include "screenReader.h"

TEST(SCREEN_LVL1, test1) {
    FigureImages gameObjectImages;
    ScreenReader screenReader(gameObjectImages);
    screenReader.read("../test/screen1.png");
}

int main(int argc, char** argv) {
    // ::testing::GTEST_FLAG(filter) = "*test5*";
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}