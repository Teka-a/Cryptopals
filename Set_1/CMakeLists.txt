cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Set_1)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()
include_directories(src tests)    

set(SOURCE src/main.cpp)

set(LIBRARIES 
    src/types.h
    src/convertions.h src/convertions.cpp
    src/common.h src/common.cpp
    src/AES.h src/AES.cpp
)

add_library(LIBS STATIC ${LIBRARIES})

add_executable(set_1 ${SOURCE})
add_executable(tests tests/tests.cpp)

target_link_libraries(set_1 LIBS)
target_link_libraries(tests LIBS GTest::gtest_main)

include(GoogleTest) 
gtest_discover_tests(tests)