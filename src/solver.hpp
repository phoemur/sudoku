#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <array>
#include <optional>
#include <utility>
#include <random>

namespace Sudoku {

using puzzle_t = std::array<std::array<int, 9>, 9>;
using namespace std;

enum class Difficulty
{
    Easy, Intermediate, Hard
};


/* Searches the grid to find an entry that is still unassigned. If
   found, a pair<row,col> is returned. Otherwise, an empty optional
   is returned */
optional<pair<size_t,size_t>> FindUnassignedLocation(const puzzle_t& grid)
{
    for (size_t row = 0; row < grid.size(); ++row)
        for (size_t col = 0; col < grid[row].size(); ++col)
            if (grid[row][col] == 0)
                return std::make_pair(row, col);

    return {};
}

/* Searches the row to find an entry that is the same as num.
   If found, a pair<row,col> is returned.
   Otherwise, an empty optional is returned */
optional<pair<size_t,size_t>> UsedInRow(const puzzle_t& grid,
                                        size_t row,
                                        int num)
{
    for (size_t col = 0; col < grid[row].size(); ++col)
        if (grid[row][col] == num)
            return make_pair(row,col);

    return {};
}

/* Searches the col to find an entry that is the same as num.
   If found, a pair<row,col> is returned.
   Otherwise, an empty optional is returned */
optional<pair<size_t,size_t>> UsedInCol(const puzzle_t& grid,
                                        size_t col,
                                        int num)
{
    for (size_t row = 0; row < grid.size(); ++row)
        if (grid[row][col] == num)
            return make_pair(row,col);

    return {};
}

/* Searches the 3x3 box to find an entry that is the same as num.
   If found, a pair<row,col> is returned.
   Otherwise, an empty optional is returned */
optional<pair<size_t,size_t>> UsedInBox(const puzzle_t& grid,
                                        std::size_t boxStartRow,
                                        std::size_t boxStartCol,
                                        int num)
{
    for (size_t row = 0; row < 3; ++row)
        for (size_t col = 0; col < 3; ++col)
            if (grid[row+boxStartRow][col+boxStartCol] == num)
                return make_pair(row+boxStartRow, col+boxStartCol);

    return {};
}

/* Returns a boolean which indicates whether it will be legal to assign
   num to the given row,col location. */
bool isSafe(const puzzle_t& grid,
            std::size_t row,
            std::size_t col,
            int num) noexcept
{
    /* Check if 'num' is not already placed in current row,
       current column and current 3x3 box */
    return !UsedInRow(grid, row, num).has_value() &&
           !UsedInCol(grid, col, num).has_value() &&
           !UsedInBox(grid, row - row%3 , col - col%3, num).has_value();
}

/* Takes a partially filled-in grid and attempts to assign values to
  all unassigned locations in such a way to meet the requirements
  for Sudoku solution (non-duplication across rows, columns, and boxes) */
bool SolveSudoku(puzzle_t& grid)
{
    // If there is no unassigned location, we are done
    auto opt = FindUnassignedLocation(grid);

    if (!opt.has_value()) // base case
    {
        return true;
    }
    else
    {
        size_t row = opt.value().first;
        size_t col = opt.value().second;

        for (int num = 1; num <= 9; ++num)
        {
            if (isSafe(grid, row, col, num)) // if looks promising
            {
                grid[row][col] = num; // make tentative assignment

                if (SolveSudoku(grid)) // recursion -> return, if success, yay!
                    return true;

                // failure, undo & try again
                grid[row][col] = 0;  // backtracking
            }
        }

        return false; // No solution exists
    }
}


/* Here it would have been better to use a book of many puzzles
   sorted by difficulty, but I decided to generate puzzles programatically.
   The risk is to underestimate the real difficulty */
puzzle_t GeneratePuzzle(Difficulty dif)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 9);
    puzzle_t grid;

    do
    {
        // Fill with zeroes
        for (auto& row : grid)
            row.fill(0);

        // Insert 10 random numbers in random positions
        int i = 0;
        while (i < 10)
        {
            auto x = static_cast<size_t>(dist(gen) - 1);
            auto y = static_cast<size_t>(dist(gen) - 1);
            int num = dist(gen);

            if (isSafe(grid, x, y, num))
            {
                grid[x][y] = num;
                ++i;
            }
        }

    }
    while (!SolveSudoku(grid));

    // Here we have a totally filled sudoku puzzle.
    // We will remove some numbers according to the difficulty
    int remove;
    switch(dif)
    {
        case Difficulty::Easy:
            remove = 36;
            break;
        case Difficulty::Intermediate:
            remove = 48;
            break;
        case Difficulty::Hard:
            remove = 56;
            break;
    }

    while (remove > 0)
    {
        auto x = static_cast<size_t>(dist(gen) - 1);
        auto y = static_cast<size_t>(dist(gen) - 1);

        if (grid[x][y] != 0)
        {
            grid[x][y] = 0;
            --remove;
        }
    }

    return grid;
}


} // End of namespace Sudoku

#endif // SOLVER_HPP
