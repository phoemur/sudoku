#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <array>
#include <type_traits>
#include "named_type.hpp"

using Puzzle_t = std::array<std::array<std::size_t, 9>, 9>;

using Row = fluent::NamedType<std::size_t, struct RowTag>;
using Col = fluent::NamedType<std::size_t, struct ColTag>;

struct Coordinate {
    Row x;
    Col y;

    // Accept in any order
    Coordinate(Row r, Col c)
        : x{r}, y{c} {}

    Coordinate(Col c, Row r)
        : x{r}, y{c} {}
};

enum class Difficulty
{
    Easy, Intermediate, Hard
};

#endif // MY_TYPES_H
