#pragma once

#include <iostream>
#include <fstream>
#include <random>

#include "Board.h"
#include "Solver.h"

class Generator
{
private:
    Solver solver;
    std::default_random_engine rng;

public:
    Board board;
    Generator(std::vector<int> numbers);
    Generator();
    ~Generator();

    void recover(Board &);

    void generate_closing(std::ofstream &, int);
    void generate_game(std::ofstream &, int);
    void generate_game(std::ofstream &, int, int);

    int _reduce_logical(int cutoff = 64);
    int _reduce_random(int cutoff = 64);
    int _reduce_recursion(int cutoff = 64);
    void _check_recursion(int &count);
};