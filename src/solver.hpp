#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <array>
#include <optional>
#include <random>

#include "my_types.h"


namespace Sudoku {

using namespace std;

/* Searches the grid to find an entry that is still unassigned. If
   found, a Coordinate is returned. Otherwise, an empty optional
   is returned */
optional<Coordinate> FindUnassignedLocation(const Puzzle_t& grid) noexcept
{
    for (size_t row = 0; row < grid.size(); ++row)
        for (size_t col = 0; col < grid[row].size(); ++col)
            if (grid[row][col] == 0)
                return Coordinate{Row(row), Col(col)};

    return {};
}

/* Searches the row to find an entry that is the same as num.
   If found, a Coordinate is returned.
   Otherwise, an empty optional is returned */
optional<Coordinate> UsedInRow(const Puzzle_t& grid,
                               Row row,
                               std::size_t num) noexcept
{
    for (size_t col = 0; col < grid[row.get()].size(); ++col)
        if (grid[row.get()][col] == num)
            return Coordinate{row, Col(col)};

    return {};
}

/* Searches the col to find an entry that is the same as num.
   If found, a Coordinate is returned.
   Otherwise, an empty optional is returned */
optional<Coordinate> UsedInCol(const Puzzle_t& grid,
                               Col col,
                               std::size_t num) noexcept
{
    for (size_t row = 0; row < grid.size(); ++row)
        if (grid[row][col.get()] == num)
            return Coordinate{Row(row),col};

    return {};
}

/* Searches the 3x3 box to find an entry that is the same as num.
   If found, a Coordinate is returned.
   Otherwise, an empty optional is returned */
optional<Coordinate> UsedInBox(const Puzzle_t& grid,
                               Row boxStartRow,
                               Col boxStartCol,
                               std::size_t num) noexcept
{
    for (size_t row = 0; row < 3; ++row)
        for (size_t col = 0; col < 3; ++col)
            if (grid[row+boxStartRow.get()][col+boxStartCol.get()] == num)
                return Coordinate{Row(row+boxStartRow.get()),
                                  Col(col+boxStartCol.get())};

    return {};
}

/* Returns a boolean which indicates whether it will be legal to assign
   num to the given row,col location. */
bool isSafe(const Puzzle_t& grid,
            Row row,
            Col col,
            std::size_t num) noexcept
{
    /* Check if 'num' is not already placed in current row,
       current column and current 3x3 box */
    return !UsedInRow(grid, row, num).has_value() &&
           !UsedInCol(grid, col, num).has_value() &&
           !UsedInBox(grid, Row(row.get() - row.get()%3), Col(col.get() - col.get()%3), num).has_value();
}

/* Takes a partially filled-in grid and attempts to assign values to
  all unassigned locations in such a way to meet the requirements
  for Sudoku solution (non-duplication across rows, columns, and boxes)
  Returns true if succeded, false otherwise */
bool SolveSudoku(Puzzle_t& grid)
{
    // If there is no unassigned location, we are done
    auto opt = FindUnassignedLocation(grid);

    if (!opt.has_value()) // base case, solution is found as we have a full grid
    {
        return true;
    }
    else
    {
        auto row = opt.value().x;
        auto col = opt.value().y;

        for (std::size_t num = 1; num <= 9; ++num)
        {
            if (isSafe(grid, row, col, num)) // if looks promising
            {
                grid[row.get()][col.get()] = num; // make tentative assignment

                if (SolveSudoku(grid)) // recursion -> return, if success, yay!
                    return true;

                // failure, undo & try again
                grid[row.get()][col.get()] = 0;  // backtracking
            }
        }

        return false; // No solution exists
    }
}


/* Here it would have been better to use a book of many puzzles
   sorted by difficulty, but I decided to generate puzzles programatically.
   The risk is to underestimate the real difficulty */
Puzzle_t GeneratePuzzle(Difficulty dif)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<std::size_t> dist(1, 9);

    Puzzle_t grid;

    do
    {
        // Fill with zeroes
        for (auto& row : grid)
            row.fill(0);

        // Insert 10 random numbers in random positions
        int i = 0;
        while (i < 10)
        {
            auto x = Row(dist(gen) - 1);
            auto y = Col(dist(gen) - 1);
            auto num = dist(gen);

            if (isSafe(grid, x, y, num))
            {
                grid[x.get()][y.get()] = num;
                ++i;
            }
        }

    }
    while (!SolveSudoku(grid));

    // Here we have a totally filled sudoku puzzle.
    // We will remove some numbers according to the difficulty
    int remove = 0;
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
        auto x = dist(gen) - 1;
        auto y = dist(gen) - 1;

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
