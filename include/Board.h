#pragma once

#include <array>
#include <vector>
#include <functional>

constexpr auto N = 3ULL;
constexpr auto UNIT = N * N;

struct Cell
{
   int row;
   int col;
   int box;
   int value;
};

class Board
{
private:
   std::array<std::array<Cell *, UNIT>, UNIT> rows;
   std::array<std::array<Cell *, UNIT>, UNIT> columns;
   std::array<std::array<Cell *, UNIT>, UNIT> boxes;

   
   void _swap_row(int row_index1, int row_index2);
   void _swap_col(int col_index1, int col_index2);
   void _swap_tower(int tower_index1, int tower_index2);
   void _swap_floor(int floor_index1, int floor_index2);
   // be true only during `_swap_tower` and `_swap_floor`
   bool _allow_swap;
   // implements for the interface `shuffle()`
   // std::array<std::function<void(Board &, int, int)>, 4>
   //     swaps{_swap_row, _swap_col, _swap_tower, _swap_floor};

// Make it easy to debug (I/O)
public:
   std::array<Cell *, UNIT * UNIT> cells;

public:
   explicit Board(std::vector<int> numbers);
   ~Board();
   std::vector<Cell *> get_hints();
   std::vector<Cell *> get_unused_cells();
   int get_unused_num();
   std::vector<int> get_candidates(Cell *cell);
   int get_possibility(Cell *cell);
   int get_peer(Cell *cell);
   int calculateDifficulty();
   void shuffle(int iter_num);
};
