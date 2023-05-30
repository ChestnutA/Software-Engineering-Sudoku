#include <iostream>
#include <cstring>
#include <unistd.h>

int c_num;
bool solving;
std::string game_path;
int game_num;
int level;
int blank_num;
bool unique_solution;

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "c:s:n:m:r:u")) != -1)
    {
        switch (opt)
        {
        case 'c':
            c_num = atoi(optarg);
            break;
        case 's':
            solving = true;
            game_path = optarg;
            break;
        case 'n':
            game_num = atoi(optarg);
            break;
        case 'm':
            level = atoi(optarg);
            break;
        case 'r':
            blank_num = atoi(optarg);
            break;
        case 'u':
            unique_solution = true;
            break;
        default:
            exit(1);
            break;
        }
    }

    return 0;
}
