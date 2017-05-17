#include "SudokuTypes.hpp"
#include "Puzzles.hpp"
#include "Solver.hpp"
#include "Validator.hpp"

#include <iostream>

int main()
{
    unsigned int failed = 0;
    unsigned int solved = 0;

    auto puzzles = getPuzzles();
    for (auto puzzle : puzzles)
    {
        Solver s{puzzle};
        auto solvedPuzzle = s.solve();

        Validator v{solvedPuzzle};

        if (v.isValid())
        {
            solved++;
        }
        else
        {
            failed++;
        }
    }

    std::cout << "\nSolved: " << solved
              << "\nFailed: " << failed << std::endl;
}
