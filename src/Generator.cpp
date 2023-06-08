#include "Generator.h"

#include <algorithm>
#include <chrono>
#include <random>

extern bool unique_solution;

Generator::Generator(std::vector<int> numbers) : board(numbers)
{
}

Generator::Generator() : board({})
{
}

Generator::~Generator()
{
}

void Generator::recover(Board &base)
{
    for (size_t i = 0; i < base.cells.size(); i++)
    {
        board.cells[i]->value = base.cells[i]->value;
    }
}

void Generator::generate_closing(int num)
{
    std::vector<int> base(UNIT * UNIT);
    std::array<int, UNIT> range;
    for (size_t i = 0; i < UNIT; i++)
    {
        range[i] = i + 1;
    }
    int round = 0, gen = 0;
    std::ofstream fout("closing.txt");
    while (true)
    {
        do
        {
            std::copy(range.begin(), range.end(), base.begin());
            base[UNIT] = range[N + round];
            solver.input_problem(base);
            solver.solve();
            solver.output_solution(fout, ++gen != num);
            if (gen == num)
                return;
        } while (std::next_permutation(range.begin(), range.end()));
        round++;
    }
}

void Generator::generate_game(std::ofstream &oup, int bottom, int top)
{
    auto blank_num = _reduce_logical(top);
    while (blank_num < bottom)
    {
        if (blank_num >= UNIT * UNIT / 2)
        {
            blank_num = _reduce_random(top);
            blank_num = _reduce_recursion(bottom + rand() % (top-bottom+1));
        }
        else
        {
            blank_num = _reduce_recursion(bottom);
        }
    }

    for (size_t i = 0; i < UNIT * UNIT; i++)
    {
        oup << (char)(board.cells[i]->value ? board.cells[i]->value + '0' : '$')
            << " \n"[i % UNIT == UNIT - 1];
    }
    oup << std::endl;
}

int Generator::_reduce_logical(int cutoff)
{
    int unused = board.get_unused_num();
    if (unused >= cutoff)
        return unused;

    auto cells = board.get_hints();
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cells.begin(), cells.end(), std::default_random_engine(seed));

    for (auto &cell : cells)
    {
        if (board.get_possibility(cell) == 1)
        {
            cell->value = 0;
            if (++unused == cutoff)
                break;
        }
    }
    return unused;
}

int Generator::_reduce_random(int cutoff)
{
    int unused = board.get_unused_num();
    if (unused >= cutoff)
        return unused;

    auto existing = board.get_hints();
    std::vector<std::pair<size_t, Cell *>> keys(existing.size());
    for (size_t i = 0; i < existing.size(); i++)
    {
        keys[i] = {board.get_peer(existing[i]), existing[i]};
    }
    std::sort(keys.begin(), keys.end());
    std::vector<Cell *> elements;
    for (auto it = keys.rbegin(); it != keys.rend(); it++)
    {
        elements.push_back(it->second);
    }

    for (auto &cell : elements)
    {
        auto original = cell->value;
        bool ambiguous = true;
        for (auto &x : board.get_candidates(cell))
        {
            if (x == original)
                continue;
            cell->value = x;

            // if solver can fill every box and the solution is valid then
            // puzzle becomes ambiguous after removing particular cell, so we can break out
            solver.input_problem(board);
            if (solver.is_unique())
            {
                cell->value = original;
                ambiguous = false;
                break;
            }
        }
        if (ambiguous)
        {
            cell->value = 0;
            if (++unused == cutoff)
                break;
        }
    }
    return unused;
}

int Generator::_reduce_recursion(int cutoff)
{
    int unused = board.get_unused_num();
    if (unused >= cutoff)
        return unused;

    auto cells = board.get_hints();
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cells.begin(), cells.end(), std::default_random_engine(seed));

    for (auto &cell : cells)
    {
        if (board.get_possibility(cell) == 1)
        {
            cell->value = 0;
            if (++unused == cutoff)
                break;
            continue;
        }
        int tmp = cell->value;
        cell->value = 0;
        int count = 0;
        _check_recursion(count);
        if (count != 1)
            cell->value = tmp;
        else if (++unused == cutoff)
            break;
    }
    return unused;
}
/// @brief
/// @param count 解的个数
void Generator::_check_recursion(int &count)
{
    Cell *cur = nullptr;
    for (auto &cell : board.cells)
    {
        if (cell->value == 0)
        {
            cur = cell;
            break;
        }
    }
    if (!cur)
    {
        count++;
        return;
    }
    // auto v = board.get_unused_cells();
    // if (v.empty())
    // {
    //     count++;
    //     return;
    // }
    // Cell *cur = v[rand() % v.size()];

    auto candidates = board.get_candidates(cur);
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(candidates.begin(), candidates.end(), std::default_random_engine(seed));
    for (auto &candidate : candidates)
    {
        cur->value = candidate;
        _check_recursion(count);
        cur->value = 0;
        if (count >= 2)
            break;
    }
}

// int main(int argc, char const *argv[])
// {
//     std::ifstream fin("../resource/sudoku.txt");
//     std::vector<int> board(UNIT * UNIT);
//     for (int i = 0; i < UNIT; i++)
//     {
//         for (int j = 0; j < UNIT; j++)
//         {
//             fin >> board[i * UNIT + j];
//         }
//     }
//     Generator gen(board);
//     std::ofstream fout("sudoku-problem.txt");    
    
//     // gen._reduce_logical();
//     // std::cout << gen.board.get_unused_cells().size() << std::endl;
//     // gen._reduce_random(15);
//     // std::cout << gen.board.get_unused_cells().size() << std::endl;
//     // gen._reduce_recursion();
//     // std::cout << gen.board.get_unused_num() << std::endl;

//     // for (int i = 0; i < UNIT; i++)
//     // {
//     //     for (int j = 0; j < UNIT; j++)
//     //     {
//     //         fout << gen.board.cells[i * UNIT + j]->value << ' ';
//     //     }
//     //     fout << std::endl;
//     // }
//     return 0;
// }
