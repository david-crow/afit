#include "header.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

int main()
{
    srand(time(NULL));

    int start_x, start_y;
    std::vector<std::vector<char>> world;

    // if user-provided filename is bad
    if (!instantiateWorld(world, start_x, start_y))
    {
        std::cout << "Unable to instantiate world! Exiting now...\n";
        return -1;
    }

    valueIteration(world);
    tdZero(world, start_x, start_y);
    tdLambda(world, start_x, start_y);

    return 0;
}
