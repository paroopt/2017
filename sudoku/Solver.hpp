#pragma once

#include "SudokuTypes.hpp"

class Solver
{
public:
    explicit Solver(Board b);

    Board solve();

private:
    void fillObvious();
    bool isSolved() const;
    Board findSolution();
    std::vector<Value> findPossibleValues(int row, int col) const;
    std::vector<Value> generateAllPossibleValues() const;
    std::vector<Value> removeValuesWhichExistInColumn(int col, std::vector<Value> values) const;
    std::vector<Value> removeValuesWhichExistInRow(int row, std::vector<Value> values) const;
    std::vector<Value> removeValuesWhichExistInBlock(int row, int col, std::vector<Value> values) const;
    Board checkPossibleSolutions(int row, int col, std::vector<Value> possibleValues);

    Board board_;
};
