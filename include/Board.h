#pragma once

#include <vector>
#include <array>

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
    std::array<std::array<Cell *, 9>, 9> rows;
    std::array<std::array<Cell *, 9>, 9> columns;
    std::array<std::array<Cell *, 9>, 9> boxes;

    void swap_row(int row_index1, int row_index2, bool allow = false);
    void swap_col(int col_index1, int col_index2, bool allow = false);
    void swap_tower(int tower_index1, int tower_index2);
    void swap_floor(int floor_index1, int floor_index2);

 public:
    std::array<Cell *, 9 * 9> cells;

 public:
    explicit Board(std::vector<int> numbers);
    ~Board();
    std::vector<Cell *> get_hints();
    std::vector<Cell *> get_unused_cells();
    int get_unused_num();
    std::vector<int> get_candidates(Cell *cell);
    int get_possibility(Cell *cell);
    size_t get_peer(Cell *cell);
    int calculateDifficulty();
    void shuffle(size_t iter_num);
};
