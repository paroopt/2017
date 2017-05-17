#!/bin/sh

#clock_defines="-DCLOCK_TYPE=std::chrono::steady_clock -DASSERT_IS_STEADY"
#clock_defines="-DCLOCK_TYPE=std::chrono::system_clock -DASSERT_IS_STEADY"
#clock_defines="-DCLOCK_TYPE=std::chrono::high_resolution_clock -DASSERT_IS_STEADY"
clock_defines="-DCLOCK_TYPE=std::chrono::high_resolution_clock"

flags="-Wall -Wextra -pedantic -std=c++14 -O3 -DNDEBUG $clock_defines"

g++ $flags Generate.cpp -o Generate
g++ $flags SetTest.cpp -o SetTest
g++ $flags SortTest.cpp -o SortTest
