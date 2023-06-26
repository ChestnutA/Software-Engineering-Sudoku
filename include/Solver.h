#pragma once

#include <iostream>
#include <fstream>

#include "Board.h"

class Solver
{
    const int FULL = (1 << (UNIT + 1)) - 2;

private:
    std::array<std::array<int, UNIT>, UNIT> board;
    std::array<int, UNIT> rows;
    std::array<int, UNIT> columns;
    std::array<int, UNIT> boxes;
    std::vector<std::pair<int, int>> blanks;
    bool valid = true;
    int res = 0;
    // be called before inputing to reset the solver
    void _reset();
    void _change(int row, int col, int digit);
    void _solve_recursion(int idx);
    void _solve_all(int idx);

public:
    void input_problem(std::ifstream &);
    void input_problem(std::vector<int>);
    void input_problem(Board &);
    template <typename T>
    void output_solution(T &oup, bool crlf = true);
    void solve();
    bool is_unique();
};