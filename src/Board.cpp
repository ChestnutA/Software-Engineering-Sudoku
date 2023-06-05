#include "Board.h"

#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <unordered_set>

Board::Board(std::vector<int> numbers)
{
    if (numbers.size() == 9 * 9)
    {
        size_t idx = 0;
        for (int row = 0; row < 9; row++)
        {
            for (int col = 0; col < 9; col++)
            {
                int box = row / 3 * 3 + col / 3;
                auto new_cell = new Cell({row, col, box, numbers[idx]});
                cells[idx++] = rows[row][col] = columns[col][row] = boxes[box][row % 3 * 3 + col % 3] = new_cell;
            }
        }
    }
    else
    {
        cells.fill(nullptr);
    }
}

Board::~Board()
{
    for (auto &&cell : cells)
        if (cell != nullptr)
            delete cell;
        else
            break;
}

std::vector<Cell *> Board::get_hints()
{
    std::vector<Cell *> hints;
    for (auto &cell : cells)
    {
        if (cell->value != 0)
        {
            hints.push_back(cell);
        }
    }
    return hints;
}

std::vector<Cell *> Board::get_unused_cells()
{
    std::vector<Cell *> unused_cells;
    for (auto &cell : cells)
    {
        if (cell->value == 0)
        {
            unused_cells.push_back(cell);
        }
    }
    return unused_cells;
}

int Board::get_unused_num()
{
    int unused_cells = 0;
    for (auto &cell : cells)
    {
        if (cell->value == 0)
        {
            unused_cells++;
        }
    }
    return unused_cells;
}

std::vector<int> Board::get_candidates(Cell *cell)
{
    std::array<bool, 9 + 1> occupation{};
    for (auto &neighbor : rows[cell->row])
    {
        occupation[neighbor->value] = true;
    }
    for (auto &neighbor : columns[cell->col])
    {
        occupation[neighbor->value] = true;
    }
    for (auto &neighbor : boxes[cell->box])
    {
        occupation[neighbor->value] = true;
    }

    std::vector<int> candidates;
    occupation[cell->value] = false;
    for (int i = 1; i <= 9; i++)
    {
        if (!occupation[i])
        {
            candidates.push_back(i);
        }
    }
    return candidates;
}

int Board::get_possibility(Cell *cell)
{
    std::array<bool, 9 + 1> occupation{};
    for (auto &neighbor : rows[cell->row])
    {
        occupation[neighbor->value] = true;
    }
    for (auto &neighbor : columns[cell->col])
    {
        occupation[neighbor->value] = true;
    }
    for (auto &neighbor : boxes[cell->box])
    {
        occupation[neighbor->value] = true;
    }

    int possibility = 9;
    occupation[cell->value] = false;
    for (int i = 1; i <= 9; i++)
    {
        possibility -= occupation[i];
    }
    return possibility;
}

size_t Board::get_peer(Cell *cell)
{
    std::unordered_set<Cell *> peer;
    for (auto &neighbor : rows[cell->row])
    {
        if (neighbor->value && neighbor->value != cell->value)
            peer.insert(neighbor);
    }
    for (auto &neighbor : columns[cell->col])
    {
        if (neighbor->value && neighbor->value != cell->value)
            peer.insert(neighbor);
    }
    for (auto &neighbor : boxes[cell->box])
    {
        if (neighbor->value && neighbor->value != cell->value)
            peer.insert(neighbor);
    }
    return peer.size();
}

void Board::swap_row(int row_index1, int row_index2, bool allow)
{
    if (allow || row_index1 / 3 == row_index2 / 3)
    {
        for (int col = 0; col < rows[row_index2].size(); col++)
        {
            int tmp = rows[row_index1][col]->value;
            rows[row_index1][col]->value = rows[row_index2][col]->value;
            rows[row_index2][col]->value = tmp;
        }
    }
}
void Board::swap_col(int col_index1, int col_index2, bool allow)
{
    if (allow || col_index1 / 3 == col_index2 / 3)
    {
        for (size_t row = 0; row < columns[col_index2].size(); row++)
        {
            int tmp = columns[col_index1][row]->value;
            columns[col_index1][row]->value = columns[col_index2][row]->value;
            columns[col_index2][row]->value = tmp;
        }
    }
}
void Board::swap_tower(int tower_index1, int tower_index2)
{
    for (int i = 0; i < 3; i++)
    {
        swap_col(tower_index1 * 3 + i, tower_index2 * 3 + i, true);
    }
}
void Board::swap_floor(int floor_index1, int floor_index2)
{
    for (int i = 0; i < 3; i++)
    {
        swap_row(floor_index1 * 3 + i, floor_index2 * 3 + i, true);
    }
}

void Board::shuffle(size_t iter_num)
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine engine(seed);
    std::uniform_int_distribution<> op(0, 3);
    std::uniform_int_distribution<> region(0, 2);
    std::uniform_int_distribution<> offset(1, 2);
    for (size_t _ = 0; _ < iter_num; _++)
    {
        int chute = region(engine) * 3,
            off1 = region(engine),
            off2 = (off1 + offset(engine)) % 3;
        switch (op(engine))
        {
        case 0:
            swap_row(chute + off1, chute + off2);
            break;
        case 1:
            swap_col(chute + off1, chute + off2);
            break;
        case 2:
            swap_tower(off1, off2);
            break;
        case 3:
            swap_floor(off1, off2);
            break;

        default:
            break;
        }
    }
}
