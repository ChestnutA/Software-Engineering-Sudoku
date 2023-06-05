#pragma once
#include "Board.h"

#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <iostream>
class Generator
{
public:
    Board board;
    Generator(std::vector<int> numbers);
    Generator();
    ~Generator();

    int _reduce_logical(int cutoff = 64);
    int _reduce_random(int cutoff = 64);
    int _reduce_recursion(int cutoff = 64);
    void _check_recursion(int &count);
};