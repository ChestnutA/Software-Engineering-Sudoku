#include "Board.h"

#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <unordered_set>

Board::Board(std::vector<int> numbers)
{
    if (numbers.size() == UNIT * UNIT)
    {
        size_t idx = 0;
        for (int row = 0; row < UNIT; row++)
        {
            for (int col = 0; col < UNIT; col++)
            {
                int box = row / N * N + col / N;
                auto new_cell = new Cell({row, col, box, numbers[idx]});
                cells[idx++] = rows[row][col] = columns[col][row] = boxes[box][row % N * N + col % N] = new_cell;
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
    std::array<bool, UNIT + 1> occupation{};
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
    for (int i = 1; i <= UNIT; i++)
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
    std::array<bool, UNIT + 1> occupation{};
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

    int possibility = UNIT;
    occupation[cell->value] = false;
    for (int i = 1; i <= UNIT; i++)
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

int Board::calculateDifficulty()
{
    std::array<int, UNIT * UNIT> store;
    for (size_t i = 0; i < UNIT * UNIT; i++)
    {
        store[i] = cells[i]->value;
    }

    int posi = 0, free = 0;
    while (true)
    {
        int modified = false;
        for (auto &blank : get_unused_cells())
        {
            if (get_possibility(blank) == 1)
            {
                blank->value = get_candidates(blank)[0];
                modified = true;
            }
        }
        if (!modified)
            break;
    }
    for (auto &blank : get_unused_cells())
    {
        int p = get_possibility(blank);
        posi += p * p;
        free += (p - 1) * (p - 1);
    }

    for (size_t i = 0; i < UNIT * UNIT; i++)
    {
        cells[i]->value = store[i];
    }
    return posi;
}

void Board::_swap_row(int row_index1, int row_index2)
{
    if (_allow_swap || row_index1 / N == row_index2 / N)
    {
        for (int col = 0; col < rows[row_index2].size(); col++)
        {
            int tmp = rows[row_index1][col]->value;
            rows[row_index1][col]->value = rows[row_index2][col]->value;
            rows[row_index2][col]->value = tmp;
        }
    }
}
void Board::_swap_col(int col_index1, int col_index2)
{
    if (_allow_swap || col_index1 / N == col_index2 / N)
    {
        for (size_t row = 0; row < columns[col_index2].size(); row++)
        {
            int tmp = columns[col_index1][row]->value;
            columns[col_index1][row]->value = columns[col_index2][row]->value;
            columns[col_index2][row]->value = tmp;
        }
    }
}
void Board::_swap_tower(int tower_index1, int tower_index2)
{
    _allow_swap = true;
    for (int i = 0; i < N; i++)
    {
        _swap_col(tower_index1 * N + i, tower_index2 * N + i);
    }
    _allow_swap = false;
}
void Board::_swap_floor(int floor_index1, int floor_index2)
{
    _allow_swap = true;
    for (int i = 0; i < N; i++)
    {
        _swap_row(floor_index1 * N + i, floor_index2 * N + i);
    }
    _allow_swap = false;
}

void Board::shuffle(size_t iter_num)
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine engine(seed);
    std::uniform_int_distribution<> op(0, 3);
    std::uniform_int_distribution<> region(0, N - 1);
    std::uniform_int_distribution<> offset(1, N - 1);
    for (size_t _ = 0; _ < iter_num; _++)
    {
        int choice = op(engine),
            chute = region(engine) * N,
            off1 = region(engine),
            off2 = (off1 + offset(engine)) % N;
        // if (choice < 2)
        // {
        //     swaps[choice](*this, chute + off1, chute + off2);
        // }
        // else
        // {
        //     swaps[choice](*this, off1, off2);
        // }
        switch (op(engine))
        {
        case 0:
            _swap_row(chute + off1, chute + off2);
            break;
        case 1:
            _swap_col(chute + off1, chute + off2);
            break;
        case 2:
            _swap_tower(off1, off2);
            break;
        case 3:
            _swap_floor(off1, off2);
            break;

        default:
            break;
        }
    }
}
