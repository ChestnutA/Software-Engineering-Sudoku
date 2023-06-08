#include "Solver.h"

void Solver::input_problem(std::ifstream &inp)
{
    _reset();

    char ch;
    for (size_t i = 0; i < UNIT; i++)
    {
        for (size_t j = 0; j < UNIT; j++)
        {
            inp >> ch;
            board[i][j] = (ch == '$' ? 0 : ch - '0');
            _change(i, j, board[i][j]);
        }
    }
    inp >> std::ws;
}
void Solver::input_problem(std::vector<int> numbers)
{
    _reset();

    size_t idx = 0;
    for (size_t i = 0; i < UNIT; i++)
    {
        for (size_t j = 0; j < UNIT; j++)
        {
            board[i][j] = numbers[idx++];
            _change(i, j, board[i][j]);
        }
    }
}
void Solver::input_problem(Board &_board)
{
    _reset();

    size_t idx = 0;
    for (size_t i = 0; i < UNIT; i++)
    {
        for (size_t j = 0; j < UNIT; j++)
        {
            board[i][j] = _board.cells[idx++]->value;
            _change(i, j, board[i][j]);
        }
    }
}

template <typename T>
void Solver::output_solution(T &oup, bool crlf)
{
    for (size_t i = 0; i < UNIT; i++)
    {
        for (size_t j = 0; j < UNIT; j++)
        {
            oup << (board[i][j] ? board[i][j] : '$')
                << " \n"[j == UNIT - 1];
        }
    }
    if (crlf)
        oup << std::endl;
}

template void Solver::output_solution(std::ofstream &oup, bool crlf);
template void Solver::output_solution(std::ostream &oup, bool crlf);

void Solver::_reset()
{
    rows.fill(0);
    columns.fill(0);
    boxes.fill(0);
    blanks.clear();
    valid = true;
    res = 0;
}

void Solver::_change(int row, int col, int digit)
{
    rows[row] ^= (1 << digit);
    columns[col] ^= (1 << digit);
    boxes[row / N * N + col / N] ^= (1 << digit);
}

void Solver::_solve_recursion(int idx)
{
    if (idx == blanks.size())
    {
        valid = true;
        return;
    }

    int i = blanks[idx].first, j = blanks[idx].second;
    int mask = ~(rows[i] | columns[j] | boxes[i / N * N + j / N]) & FULL;
    for (; mask && !valid; mask &= (mask - 1))
    {
        int digit_mask = mask & (-mask);
        int digit = __builtin_ctz(digit_mask);
        _change(i, j, digit);
        board[i][j] = digit;
        _solve_recursion(idx + 1);
        _change(i, j, digit);
    }
}

void Solver::solve()
{
    if (!valid)
    {
        std::cout << "Error\n";
    }
    valid = false;

    while (true)
    {
        int modified = false;
        for (size_t i = 0; i < UNIT; ++i)
        {
            for (size_t j = 0; j < UNIT; ++j)
            {
                if (board[i][j] == 0)
                {
                    int mask = ~(rows[i] | columns[j] | boxes[i / N * N + j / N]) & FULL;
                    if (!(mask & (mask - 1)))
                    {
                        int digit = __builtin_ctz(mask);
                        _change(i, j, digit);
                        board[i][j] = digit;
                        modified = true;
                    }
                }
            }
        }
        if (!modified)
            break;
    }

    for (size_t i = 0; i < UNIT; ++i)
    {
        for (size_t j = 0; j < UNIT; ++j)
        {
            if (board[i][j] == 0)
            {
                blanks.emplace_back(i, j);
            }
        }
    }

    _solve_recursion(0);
}

void Solver::_solve_all(int idx)
{
    if (idx == blanks.size())
    {
        res++;
        return;
    }

    int i = blanks[idx].first, j = blanks[idx].second;
    int mask = ~(rows[i] | columns[j] | boxes[i / N * N + j / N]) & FULL;
    for (; mask; mask &= (mask - 1))
    {
        int digit_mask = mask & (-mask);
        int digit = __builtin_ctz(digit_mask);
        _change(i, j, digit);
        board[i][j] = digit;
        _solve_all(idx + 1);
        _change(i, j, digit);
    }
}

bool Solver::is_unique()
{
    res = 0;

    while (true)
    {
        int modified = false;
        for (size_t i = 0; i < UNIT; ++i)
        {
            for (size_t j = 0; j < UNIT; ++j)
            {
                if (board[i][j] == 0)
                {
                    int mask = ~(rows[i] | columns[j] | boxes[i / N * N + j / N]) & FULL;
                    if (!(mask & (mask - 1)))
                    {
                        int digit = __builtin_ctz(mask);
                        _change(i, j, digit);
                        board[i][j] = digit;
                        modified = true;
                    }
                }
            }
        }
        if (!modified)
            break;
    }

    for (size_t i = 0; i < UNIT; ++i)
    {
        for (size_t j = 0; j < UNIT; ++j)
        {
            if (board[i][j] == 0)
            {
                blanks.emplace_back(i, j);
            }
        }
    }

    _solve_all(0);
    // if (res == 0) std::cout << "No solution." << std::endl;
    return res == 1;
}

// int main()
// {
//     std::ifstream fin("../game.txt");
//     std::ofstream fout("../solution.txt");
//     Solver solver;
//     while (!fin.eof())
//     {
//         solver.input_problem(fin);
//         solver.solve();
//         solver.output_solution(fout, !fin.eof());
//     }
// }
