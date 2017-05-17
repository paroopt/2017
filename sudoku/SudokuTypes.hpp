#pragma once

#include <vector>

using Value = unsigned int;
using Board = std::vector<std::vector<Value>>;
using Block = std::vector<std::vector<Value>>;
using Size = unsigned int;

constexpr Size BOARD_SIZE = 9;
constexpr Size BLOCK_SIZE = 3;
constexpr Value EMPTY_FIELD = 0;
