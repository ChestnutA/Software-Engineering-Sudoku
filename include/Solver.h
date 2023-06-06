#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include "Board.h"

class Solver
{
    const int FULL = (1 << (9 + 1)) - 2;

private:
    std::array<std::array<int, 9>, 9> board;
    std::array<int, 9> rows;
    std::array<int, 9> columns;
    std::array<int, 9> boxes;
    std::vector<std::pair<int, int>> blanks;
    bool valid = true;
    int res = 0;
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