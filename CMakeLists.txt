cmake_minimum_required(VERSION 3.0.0)
project(sber-mahjong VERSION 1.0.0)

SET(CMAKE_CXX_STANDARD 17)
# SET(CMAKE_CXX_FLAGS_RELEASE "-O2")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
message(${OpenCV_INCLUDE_DIRS})

include_directories(include)
add_executable(
    sber-mahjong
    src/main.cpp
    src/card.cpp
    src/screenReader.cpp
    src/figureImages.cpp
)

enable_testing()
add_executable(
    sber-mahjong-test
    test/test.cpp
    src/card.cpp
    src/screenReader.cpp
    src/figureImages.cpp
)

target_link_libraries(
    sber-mahjong-test
    GTest::gtest_main
    GTest::gmock
)

target_link_libraries(sber-mahjong-test ${OpenCV_LIBS})
target_link_libraries(sber-mahjong ${OpenCV_LIBS})

include(GoogleTest)
gtest_discover_tests(sber-mahjong-test)