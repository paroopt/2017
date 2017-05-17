#pragma once

#include "SudokuTypes.hpp"

class Validator
{
public:
    explicit Validator(Board b);

    bool isValid();

private:
    bool isValidValue(int x);
    Block getBlock(Board b, int index);
    bool checkBlock(Block block);
    std::vector<Value> getColumn(Board b, int index);
    bool checkColumn(std::vector<Value> col);
    std::vector<Value> getRow(Board b, int index);
    bool checkRow(std::vector<Value> row);
    bool checkResults(std::vector<bool> blockValidationResults, std::vector<bool> columnValidationResults, std::vector<bool> rowValidationResults);

    Board board_;
};
