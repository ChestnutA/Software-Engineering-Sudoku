#include "Generator.h"
#include <iterator>

Generator::Generator(std::vector<int> numbers) : board(numbers)
{
}

Generator::Generator() : board({})
{
}

Generator::~Generator()
{
}

int Generator::_reduce_logical(int cutoff)
{
    auto cells = board.get_hints();
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cells.begin(), cells.end(), std::default_random_engine(seed));

    for (auto &cell : cells)
    {
        if (board.get_candidates(cell).size() == 1)
        {
            cell->value = 0;
        }
    }
    return 0;
}

int Generator::_reduce_random(int cutoff)
{
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
            if (hasUniqueSolution(board))
            {
                cell->value = original;
                ambiguous = false;
                break;
            }
        }
        if (ambiguous)
        {
            cell->value = 0;
        }
    }
    return 0;
}

int Generator::_reduce_recursion(int cutoff)
{
    auto cells = board.get_hints();
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cells.begin(), cells.end(), std::default_random_engine(seed));

    for (auto &cell : cells)
    {
        if (board.get_candidates(cell).size() == 1)
        {
            cell->value = 0;
            continue;
        }
        int tmp = cell->value;
        cell->value = 0;
        int count = 0;
        _check_recursion(count);
        if (count != 1)
            cell->value = tmp;
    }
    return 0;
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

int main(int argc, char const *argv[])
{
    std::ifstream fin("../resource/sudoku.txt");
    std::vector<int> board(9 * 9);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fin >> board[i * 9 + j];
        }
    }
    Generator gen(board);
    std::ofstream fout("sudoku-problem.txt");    
    
    gen._reduce_logical();
    std::cout << gen.board.get_unused_cells().size() << std::endl;
    gen._reduce_random(15);
    std::cout << gen.board.get_unused_cells().size() << std::endl;
    gen._reduce_recursion();
    std::cout << gen.board.get_unused_cells().size() << std::endl;

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fout << gen.board.cells[i * 9 + j]->value << ' ';
        }
        fout << std::endl;
    }
    return 0;
}
