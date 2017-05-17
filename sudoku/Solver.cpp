#include "Solver.hpp"

#include <algorithm>
#include <vector>

Solver::Solver(Board b)
{
    board_ = b;
}

Board Solver::solve()
{
    fillObvious();

    if (isSolved())
    {
        return board_;
    }
    else
    {
        return findSolution();
    }
}

void Solver::fillObvious()
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (board_[row][col] == EMPTY_FIELD)
            {
                auto possibleValues = findPossibleValues(row, col);
                if (possibleValues.size() == 1)
                {
                    board_[row][col] = possibleValues[0];
                }
            }
        }
    }
}

bool Solver::isSolved() const
{
    bool emptyFound = false;

    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (board_[row][col] == EMPTY_FIELD)
            {
                emptyFound = true;
            }
        }
    }

    return !emptyFound;
}

Board Solver::findSolution()
{
    for (int col = 0; col < BOARD_SIZE; col++)
    {
        for (int row = 0; row < BOARD_SIZE; row++)
        {
            if (board_[row][col] == EMPTY_FIELD)
            {
                auto possibleValues = findPossibleValues(row, col);
                return checkPossibleSolutions(row, col, possibleValues);
            }
        }
    }

    return {};
}

std::vector<Value> Solver::findPossibleValues(int row, int col) const
{
    auto allPossibleValues = generateAllPossibleValues();

    allPossibleValues = removeValuesWhichExistInColumn(col, allPossibleValues);
    allPossibleValues = removeValuesWhichExistInRow(row, allPossibleValues);
    allPossibleValues = removeValuesWhichExistInBlock(row, col, allPossibleValues);

    return allPossibleValues;
}

std::vector<Value> Solver::generateAllPossibleValues() const
{
    std::vector<Value> values;
    for (Value i = 1; i <= BOARD_SIZE; i++)
    {
        values.push_back(i);
    }

    return values;
}

std::vector<Value> Solver::removeValuesWhichExistInColumn(int col, std::vector<Value> values) const
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        auto iter = std::find(values.begin(), values.end(), board_[i][col]);
        if (iter != values.end())
        {
            values.erase(iter);
        }
    }

    return values;
}

std::vector<Value> Solver::removeValuesWhichExistInRow(int row, std::vector<Value> values) const
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        auto iter = std::find(values.begin(), values.end(), board_[row][i]);
        if (iter != values.end())
        {
            values.erase(iter);
        }
    }

    return values;
}

std::vector<Value> Solver::removeValuesWhichExistInBlock(int row, int col, std::vector<Value> values) const
{
    int blockBeginInColumn = (col / BLOCK_SIZE) * BLOCK_SIZE;
    int blockEndInColumn = blockBeginInColumn + BLOCK_SIZE;
    int blockBeginInRow = (row / BLOCK_SIZE) * BLOCK_SIZE;
    int blockEndInRow = blockBeginInRow + BLOCK_SIZE;

    for (int j = blockBeginInColumn; j < blockEndInColumn; j++)
    {
        for (int i = blockBeginInRow; i < blockEndInRow; i++)
        {
            auto iter = std::find(values.begin(), values.end(), board_[i][j]);
            if (iter != values.end())
            {
                values.erase(iter);
            }
        }
    }

    return values;
}

Board Solver::checkPossibleSolutions(int row, int col, std::vector<Value> possibleValues)
{
    for (auto value : possibleValues)
    {
        auto newBoard = board_;
        newBoard[row][col] = value;

        Solver s{newBoard};
        auto solution = s.solve();

        if (!solution.empty())
        {
            return solution;
        }
    }

    return {};
}
