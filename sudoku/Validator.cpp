#include "Validator.hpp"

#include <algorithm>
#include <set>
#include <vector>

Validator::Validator(Board b)
{
    board_ = b;
}

bool Validator::isValid()
{
    Block block;
    std::vector<Value> column;
    std::vector<Value> row;
    bool isValidSize = true;
    bool isValidBlock = true;
    bool isValidRow = true;
    bool isValidColums = true;
    std::vector<bool> blockValidationResults;
    std::vector<bool> columnValidationResults;
    std::vector<bool> rowValidationResults;

    if (board_.size() != 9)
    {
        return false;
    }

    for (int i = 0; i < 9; i++)
    {
        auto row = board_[i];
        isValidSize = isValidSize && (row.size() == 9);
    }

    if (!isValidSize)
    {
        return false;
    }

    for (unsigned int col = 0; col < BOARD_SIZE; col++)
    {
        for (unsigned int row = 0; row < BOARD_SIZE; row++)
        {
            if (!isValidValue(board_[row][col]))
                return false;
        }
    }

    for (unsigned int i = 0; i < 9; i++)
    {
        block = getBlock(board_, i);
        blockValidationResults.push_back(checkBlock(block));
    }

    for (unsigned int i = 0; i < 9; i++)
    {
        column = getColumn(board_, i);
        columnValidationResults.push_back(checkColumn(column));
    }

    for (int j = 0; j < 9; j++)
    {
        row = getRow(board_, j);
        rowValidationResults.push_back(checkRow(row));
    }

    return checkResults(blockValidationResults, columnValidationResults, rowValidationResults);

}

bool Validator::isValidValue(int x)
{
    bool isPositive = (x > 0);
    return isPositive && (x < 10);
}

Block Validator::getBlock(Board b, int index)
{
    Block blck;

    if (index == 0)
    {
        blck.push_back({b[0][0], b[0][1], b[0][2]});
        blck.push_back({b[1][0], b[1][1], b[1][2]});
        blck.push_back({b[2][0], b[2][1], b[2][2]});
    }
    else if (index == 1)
    {
        blck.push_back({b[0][3], b[0][4], b[0][5]});
        blck.push_back({b[1][3], b[1][4], b[1][5]});
        blck.push_back({b[2][3], b[2][4], b[2][5]});
    }
    else if (index == 2)
    {
        blck.push_back({b[0][6], b[0][7], b[0][8]});
        blck.push_back({b[1][6], b[1][7], b[1][8]});
        blck.push_back({b[2][6], b[2][7], b[2][8]});
    }
    else if (index == 3)
    {
        blck.push_back({b[3][0], b[3][1], b[3][2]});
        blck.push_back({b[4][0], b[4][1], b[4][2]});
        blck.push_back({b[5][0], b[5][1], b[5][2]});
    }
    else if (index == 4)
    {
        blck.push_back({b[3][3], b[3][4], b[3][5]});
        blck.push_back({b[4][3], b[4][4], b[4][5]});
        blck.push_back({b[5][3], b[5][4], b[5][5]});
    }
    else if (index == 5)
    {
        blck.push_back({b[3][6], b[3][7], b[3][8]});
        blck.push_back({b[4][6], b[4][7], b[4][8]});
        blck.push_back({b[5][6], b[5][7], b[5][8]});
    }
    else if (index == 6)
    {
        blck.push_back({b[6][0], b[6][1], b[6][2]});
        blck.push_back({b[7][0], b[7][1], b[7][2]});
        blck.push_back({b[8][0], b[8][1], b[8][2]});
    }
    else if (index == 7)
    {
        blck.push_back({b[6][3], b[6][4], b[6][5]});
        blck.push_back({b[7][3], b[7][4], b[7][5]});
        blck.push_back({b[8][3], b[8][4], b[8][5]});
    }
    else if (index == 8)
    {
        blck.push_back({b[6][6], b[6][7], b[6][8]});
        blck.push_back({b[7][6], b[7][7], b[7][8]});
        blck.push_back({b[8][6], b[8][7], b[8][8]});
    }

    return blck;
}

bool Validator::checkBlock(Block block)
{
    std::set<int> values;

    for (auto line : block)
    {
        values.insert(line[0]);
        values.insert(line[1]);
        values.insert(line[2]);
    }

    return values.size() == 9;
}

std::vector<Value> Validator::getColumn(Board b, int index)
{
    std::vector<Value> c;
    for (auto row : b)
    {
        c.push_back(row[index]);
    }

    return c;
}

bool Validator::checkColumn(std::vector<Value> col)
{
    std::set<int> unique;
    for (auto x : col)
    {
        unique.insert(x);
    }

    return unique.size() == 9;
}

std::vector<Value> Validator::getRow(Board b, int index)
{
    return b[index];
}

bool Validator::checkRow(std::vector<Value> row)
{
    auto it = std::unique(row.begin(), row.end());
    row.erase(it, row.end());
    return row.size();
}

bool Validator::checkResults(std::vector<bool> blockValidationResults, std::vector<bool> columnValidationResults, std::vector<bool> rowValidationResults)
{
    for (auto x : blockValidationResults)
        if (!x) return false;

    for (auto y : columnValidationResults)
        if (!y) return false;

    for (auto z : rowValidationResults)
        if (!z) return false;

    return true;
}
