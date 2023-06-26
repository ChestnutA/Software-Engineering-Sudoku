#include <cstring>
#include <cstdlib>

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

#include <unistd.h>

#include "Board.h"
#include "Solver.h"
#include "Generator.h"

bool op_c, op_s, op_n, op_m, op_r, op_u;
std::vector<int> opts;
std::map<int, int> opt2order;
int c_num;
int game_num;
int level;
std::pair<int, int> blank_num;
bool unique_solution;

int main(int argc, char *argv[])
{
    std::string game_path;
    int opt, order = 0;
    char *_Context = nullptr;
    while ((opt = getopt(argc, argv, "c:s:n:m:r:u")) != -1)
    {
        opts.push_back(opt);
        opt2order[opt] = order++;
        switch (opt)
        {
        case 'c':
            op_c = true;
            c_num = atoi(optarg);
            // 1-1000000
            break;
        case 's':
            op_s = true;
            game_path = optarg;
            break;
        case 'n':
            op_n = true;
            game_num = atoi(optarg);
            // 1-10000
            break;
        case 'm':
            op_m = true;
            level = atoi(optarg);
            break;
        case 'r':
            op_r = true;
            blank_num.first = atoi(strtok_s(optarg, "-", &_Context));
            blank_num.second = atoi(strtok_s(NULL, "-", &_Context));
            break;
        case 'u':
            op_u = true;
            unique_solution = true;
            break;
        default:
            exit(1);
            break;
        }
    }

    if (!opts.size())
    {
        std::cout << "This program has nothing to do." << std::endl;
        return 0;
    }

    // check arguments
    for (size_t i = 0; i < opts.size(); i++)
    {
        if (opt2order[opts[i]] != i)
            continue;
        switch (opts[i])
        {
        case 'c':
            if (!(1 <= c_num && c_num <= 1000000))
            {
                std::cerr
                    << "Value of argument c should be in [1, 1000000]."

                    << std::endl;
                c_num = std::max(c_num, 1);
                c_num = std::min(c_num, 1000000);
            }
            // 1-1000000
            break;
        case 'n':
            if (!(1 <= game_num && game_num <= 10000))
            {
                std::cerr

                    << "Value of argument n should be in [1, 10000]."

                    << std::endl;
                game_num = std::max(game_num, 1);
                game_num = std::min(game_num, 10000);
            }
            // 1-10000
            break;
        case 'm':
            if (!op_n)
                std::cerr
                    << "Usage of argument m requires the argument n."
                    << std::endl;
            if (!(1 <= level && level <= 3))
            {
                std::cerr
                    << "Value of argument m should be 1, 2 or 3."
                    << std::endl;
                level = std::max(level, 1);
                level = std::min(level, 3);
            }
            break;
        case 'r':
            if (!op_n)
                std::cerr
                    << "Usage of argument r requires the argument n."
                    << std::endl;
            if (blank_num.first > blank_num.second)
            {
                std::cerr
                    << "Value of argument r is invalid."
                    << std::endl;
                std::swap(blank_num.first, blank_num.second);
            }
            if (!(20 <= blank_num.first && blank_num.first <= 55) || !(20 <= blank_num.second && blank_num.second <= 55))
            {
                std::cerr
                    << "Value of argument r should be in [20, 55]."
                    << std::endl;
                blank_num.first = std::max(blank_num.first, 20);
                blank_num.second = std::min(blank_num.second, 55);
            }
            break;
        case 'u':
            if (!op_n)
                std::cerr
                    << "Usage of argument u requires the argument n."
                    << std::endl;
            break;
        default:
            break;
        }
    }

    // solve problems from `game_path`
    if (op_s)
    {
        std::ifstream fin(game_path);
        if (fin.fail())
        {
            std::cerr
                << "Unable to open '" << game_path << "'."
                << std::endl;
        }
        std::ofstream fout("solution.txt");
        Solver solver;
        while (!fin.eof())
        {
            solver.input_problem(fin);
            solver.solve();
            solver.output_solution(fout, !fin.eof());
        }
    }

    // generate closeing games
    if (op_c)
    {
        std::ofstream fout("closing.txt");
        Generator generator;
        generator.generate_closing(fout, c_num);
    }

    // Generate games
    if (op_n)
    {
        std::ofstream fout("problems.txt");
        std::vector<int> base = {
            2, 3, 1, 4, 5, 6, 8, 9, 7,
            5, 6, 4, 7, 8, 9, 2, 3, 1,
            8, 9, 7, 1, 2, 3, 5, 6, 4,
            7, 8, 9, 5, 3, 1, 4, 2, 6,
            3, 1, 5, 6, 4, 2, 7, 8, 9,
            4, 2, 6, 9, 7, 8, 3, 1, 5,
            1, 4, 2, 3, 6, 5, 9, 7, 8,
            6, 5, 3, 8, 9, 7, 1, 4, 2,
            9, 7, 8, 2, 1, 4, 6, 5, 3};
        Board board(base);
        Generator generator(base);
        if (op_r)
        {
            generator.generate_game(fout, blank_num.first, blank_num.second);
            for (size_t _ = 1; _ < game_num; _++)
            {
                board.shuffle(rand() % 20);
                generator.recover(board);
                generator.generate_game(fout, blank_num.first, blank_num.second);
            }
        }
        else if (op_m)
        {
            generator.generate_game(fout, level);
            for (size_t _ = 1; _ < game_num; _++)
            {
                board.shuffle(rand() % 20);
                generator.recover(board);
                generator.generate_game(fout, level);
            }
        }
        else
        {
            generator.generate_game(fout, 20, 55);
            for (size_t _ = 1; _ < game_num; _++)
            {
                board.shuffle(rand() % 20);
                generator.recover(board);
                generator.generate_game(fout, 20, 55);
            }
        }
    }

    system("pause");
    return 0;
}

/*
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>

int __cdecl main()
{
   LPWSTR *szArglist;
   int nArgs;
   int i;

   szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
   if( NULL == szArglist )
   {
      wprintf(L"CommandLineToArgvW failed\n");
      return 0;
   }
   //else for( i=0; i<nArgs; i++) printf("%d: %ws\n", i, szArglist[i]);
   else for( i=0; i<nArgs; i++) wprintf(L"%d: %ls\n", i, szArglist[i]);

// Free memory allocated for CommandLineToArgvW arguments.

   LocalFree(szArglist);

   return(1);
}
*/
