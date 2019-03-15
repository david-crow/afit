#include "header.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>

// file output
const char outfile[] = "policies.txt";

// various parameters
int REWARD = -1; // we set this later
const double ERROR = 0.1;
const double DISCOUNT = 0.7;
const double LEARNING = 0.3;
const double LAMBDA = 0.5;

void valueIteration(std::vector<std::vector<char>>& world)
{
    // we need these tables
    std::vector<std::vector<char>> moves(world.size());
    std::vector<std::vector<double>> utilities(world.size());
    initializeTables(moves, utilities, world);

    // ensure we don't overwrite our utilities mid-calculation
    std::vector<std::vector<double>> new_utilities(world.size());
    for (int i = 0; i < new_utilities.size(); i++)
        new_utilities[i].resize(new_utilities.size());

    for (int k = 1; k <= pow(static_cast<int>(world.size()), 2); k++)
    {
        double delta = 0;

        for (int x = 0; x < world.size(); x++)
        {
            for (int y = 0; y < world.size(); y++)
            {
                // we don't care about invalid states
                if (world[y][x] != 'O')
                {
                    // compute and argmax the NSEW actions/utilities and update the tables with the best action/utility
                    ActionValues action_utilities = evaluateActionValues(x, y, utilities, world);
                    auto action_value = evaluateBestAction(action_utilities);
                    moves[y][x] = action_value.first;
                    new_utilities[y][x] = (world[y][x] == 'G' ? REWARD : 0) - 1 + DISCOUNT * action_value.second;

                    // update delta
                    if (abs(utilities[y][x] - new_utilities[y][x]) > delta)
                        delta = abs(utilities[y][x] - new_utilities[y][x]) > delta;
                }
            }
        }

        // copy new utilities into the current utility table
        for (int i = 0; i < world.size(); i++)
            for (int j = 0; j < world.size(); j++)
                utilities[i][j] = new_utilities[i][j];

        // if our values have converged
        if (delta < (ERROR * (1 - DISCOUNT) / DISCOUNT))
            break;
    }

    write(moves, world, "Value Iteration");
    return;
}

void tdZero(std::vector<std::vector<char>>& world, const int& start_x, const int& start_y)
{
    std::ofstream out("tdZero.csv");
    out << "Iterations,TD(0) Path Length,Reward\n";

    // we need these tables
    std::vector<std::vector<char>> moves(world.size());
    std::vector<std::vector<double>> utilities(world.size());
    initializeTables(moves, utilities, world);

    // possible actions
    const char actions[4] = {'N', 'S', 'E', 'W'};

    for (int k = 1; k <= pow(static_cast<int>(world.size()), 2); k++)
    {
        int x, y;
        do
        {
            x = rand() % world.size();
            y = rand() % world.size();
        } while (world[y][x] == 'O');

        double epsilon = 100. / k;
        int num_moves = 0;

        do
        {
            std::pair<int, int> move_to;

            // compute and argmax the NSEW actions/utilities and update the tables with the best action/utility
            ActionValues action_utilities = evaluateActionValues(x, y, utilities, world);
            moves[y][x] = evaluateBestAction(action_utilities).first;

            // with probability of (1 - ϵ), make the optimal move
            char direction;
            if (rand() % 100 < (100 - epsilon))
                direction = moves[y][x];
            else
                direction = actions[rand() % 4];

            move_to = makeMove(direction, x, y, world);

            utilities[y][x] += LEARNING * (
                (world[move_to.second][move_to.first] == 'G' ? REWARD : 0) - 1
                + DISCOUNT * utilities[move_to.second][move_to.first] - utilities[y][x]);

            x = move_to.first;
            y = move_to.second;
            num_moves++;
        } while (world[y][x] != 'G' && num_moves < pow(static_cast<int>(world.size()), 2));

        // every so often, evaluate the policy
        if (k % 5 == 0)
        {
            auto result = evaluatePolicy(world, moves, start_x, start_y);
            out << k << "," << result.first << "," << result.second << "\n";
        }
    }

    write(moves, world, "TD(0)");
    out.close();
    return;
}

void tdLambda(std::vector<std::vector<char>>& world, const int& start_x, const int& start_y)
{
    std::ofstream out("tdLambda.csv");
    out << "Iterations,TD(λ) Path Length,Reward\n";

    // we need these tables
    std::vector<std::vector<char>> moves(world.size());
    std::vector<std::vector<double>> utilities(world.size());
    initializeTables(moves, utilities, world);

    // for TD(λ), we also need this one
    std::vector<std::vector<double>> eligibility(world.size());
    for (int i = 0; i < world.size(); i++)
    {
        eligibility[i].resize(world.size());

        for (int j = 0; j < world.size(); j++)
            eligibility[i][j] = 0;
    }

    // possible actions
    const char actions[4] = {'N', 'S', 'E', 'W'};

    for (int k = 1; k <= pow(static_cast<int>(world.size()), 2); k++)
    {
        int x, y;
        do
        {
            x = rand() % world.size();
            y = rand() % world.size();
        } while (world[y][x] == 'O');

        double epsilon = 100. / k;
        int num_moves = 0;

        do
        {
            std::pair<int, int> move_to;

            // compute and argmax the NSEW actions/utilities and update the tables with the best action/utility
            ActionValues action_utilities = evaluateActionValues(x, y, utilities, world);
            moves[y][x] = evaluateBestAction(action_utilities).first;

            // with probability of (1 - ϵ), make the optimal move
            char direction;
            if (rand() % 100 < (100 - epsilon))
                direction = moves[y][x];
            else
                direction = actions[rand() % 4];

            move_to = makeMove(direction, x, y, world);
            eligibility[y][x] += 1;
            double delta = (world[move_to.second][move_to.first] == 'G' ? REWARD : 0) - 1
                + DISCOUNT * utilities[move_to.second][move_to.first] - utilities[y][x];

            for (int i = 0; i < world.size(); i++)
            {
                for (int j = 0; j < world.size(); j++)
                {
                    utilities[y][x] += LEARNING * delta * eligibility[y][x];
                    eligibility[y][x] *= DISCOUNT * LAMBDA;
                }
            }

            x = move_to.first;
            y = move_to.second;
            num_moves++;
        } while (world[y][x] != 'G' && num_moves < pow(static_cast<int>(world.size()), 2));

        // every so often, evaluate the policy
        if (k % 5 == 0)
        {
            auto result = evaluatePolicy(world, moves, start_x, start_y);
            out << k << "," << result.first << "," << result.second << "\n";
        }
    }

    write(moves, world, "TD(λ)");
    out.close();

    return;
}

std::pair<int, int> evaluatePolicy(const std::vector<std::vector<char>>& world,
    const std::vector<std::vector<char>>& policy, const int& start_x, const int& start_y)
{
    int x = start_x, y = start_y;
    int num_moves = 0;
    int earned_reward = 0;

    do
    {
        // make the suggested move
        auto move_to = makeMove(policy[y][x], x, y, world);
        x = move_to.first;
        y = move_to.second;

        // update the reward and the path length
        earned_reward += (world[y][x] == 'G' ? REWARD : 0) - 1;
        num_moves++;
    } while (world[y][x] != 'G' && num_moves < world.size() * world.size() / 2.);

    return std::make_pair(num_moves, earned_reward);
}

ActionValues evaluateActionValues(const int& x, const int& y,
    const std::vector<std::vector<double>>& utilities, const std::vector<std::vector<char>>& world)
{
    ActionValues a;

    // figure out where each action actually takes us
    auto north = northCell(x, y, world);
    auto south = southCell(x, y, world);
    auto east = eastCell(x, y, world);
    auto west = westCell(x, y, world);

    a.north = 0.90 * utilities[north.second][north.first] + 0.07 * utilities[y][x] + 0.03 * utilities[south.second][south.first];
    a.south = 0.90 * utilities[south.second][south.first] + 0.07 * utilities[y][x] + 0.03 * utilities[north.second][north.first];
    a.east = 0.90 * utilities[east.second][east.first] + 0.07 * utilities[y][x] + 0.03 * utilities[west.second][west.first];
    a.west = 0.90 * utilities[west.second][west.first] + 0.07 * utilities[y][x] + 0.03 * utilities[east.second][east.first];

    return a;
}

std::pair<int, int> makeMove(const char& direction, int x, int y, const std::vector<std::vector<char>>& world)
{
    int probability = rand() % 100;

    if (probability < 90)
    {
        switch(direction)
        {
        case 'N':
            if (y + 1 < world.size() && world[y + 1][x] != 'O')
                y++;
            break;
        case 'S':
            if (y - 1 >= 0 && world[y - 1][x] != 'O')
                y--;
            break;
        case 'E':
            if (x + 1 < world.size() && world[y][x + 1] != 'O')
                x++;
            break;
        case 'W':
            if (x - 1 >= 0 && world[y][x - 1] != 'O')
                x--;
        }
    }
    else if (probability < 93)
    {
        switch(direction)
        {
        case 'N':
            if (y - 1 >= 0 && world[y - 1][x] != 'O')
                y--;
            break;
        case 'S':
            if (y + 1 < world.size() && world[y + 1][x] != 'O')
                y++;
            break;
        case 'E':
            if (x - 1 >= 0 && world[y][x - 1] != 'O')
                x--;
        case 'W':
            if (x + 1 < world.size() && world[y][x + 1] != 'O')
                x++;
            break;
        }
    }

    return std::make_pair(x, y);
}

std::pair<char, double> evaluateBestAction(const ActionValues& a)
{
    char best_move = 'N';
    double max_utility = a.north;

    if (a.south > max_utility)
    {
        max_utility = a.south;
        best_move = 'S';
    }

    if (a.east > max_utility)
    {
        max_utility = a.east;
        best_move = 'E';
    }

    if (a.west > max_utility)
    {
        max_utility = a.west;
        best_move = 'W';
    }

    return std::make_pair(best_move, max_utility);
}

std::pair<int,int> northCell(const int& x, const int& y, const std::vector<std::vector<char>>& world)
{
    // if we can actually move north
    if (y + 1 < world.size() && world[y + 1][x] != 'O')
        return std::make_pair(x, y + 1);
    return std::make_pair(x, y);
}

std::pair<int,int> southCell(const int& x, const int& y, const std::vector<std::vector<char>>& world)
{
    // if we can actually move south
    if (y - 1 >= 0 && world[y - 1][x] != 'O')
        return std::make_pair(x, y - 1);
    return std::make_pair(x, y);
}

std::pair<int,int> eastCell(const int& x, const int& y, const std::vector<std::vector<char>>& world)
{
    // if we can actually move east
    if (x + 1 < world.size() && world[y][x + 1] != 'O')
        return std::make_pair(x + 1, y);
    return std::make_pair(x, y);
}

std::pair<int,int> westCell(const int& x, const int& y, const std::vector<std::vector<char>>& world)
{
    // if we can actually move west
    if (x - 1 >= 0 && world[y][x - 1] != 'O')
        return std::make_pair(x - 1, y);
    return std::make_pair(x, y);
}

void initializeTables(std::vector<std::vector<char>>& moves,
    std::vector<std::vector<double>>& utilities, const std::vector<std::vector<char>>& world)
{
    for (int i = 0; i < world.size(); i++)
    {
        moves[i].resize(world.size());
        utilities[i].resize(world.size());
    }

    // reset the world values
    for (int i = 0; i < world.size(); i++)
    {
        for (int j = 0; j < world.size(); j++)
        {
            moves[i][j] = world[i][j] == 'O' ? '-' : 'N';
            utilities[i][j] = 0;
        }
    }
}

bool instantiateWorld(std::vector<std::vector<char>>& world, int& start_x, int& start_y)
{
    char generate_world;

    std::cout << "Welcome to GridWorld!\n\nDo you want me to generate a world for you? (y/n): ";
    std::cin >> generate_world;

    // if the user does not have a file, generate a world
    if (generate_world == 'y')
    {
        int size;

        do
        {
            std::cout << "Enter your world size: ";
            std::cin >> size;
        } while (size < 3);
        
        generateWorld(size, world);
    }
    else
    {
        std::string filename;
        std::cout << "Enter your filename: ";
        std::cin >> filename;

        if (!readFile(world, filename))
            return false;
    }


    // output for convenience
    std::cout << "\nWorld\n\n";
    print(world);

    // ask the user if he wants to place the agent himself
    setStartLocation(world, start_x, start_y);

    // set the reward for reaching the goal
    REWARD = pow(static_cast<int>(world.size()), 2) / 2.;

    return true;
}

void generateWorld(const int& size, std::vector<std::vector<char>>& world)
{
    world.resize(size);
    for (int i = 0; i < size; i++)
        world[i].resize(size);

    // each square has a 20% chance of containing an obstacle and an 80% chance of being vacant
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            world[i][j] = rand() % 100 >= 20 ? 'V' : 'O';

    world[rand() % size][rand() % size] = 'G';
    writeWorldToFile(world);
    return;
}

void setStartLocation(std::vector<std::vector<char>>& world, int& start_x, int& start_y)
{
    char generate_agent;
    std::cout << "Do you want me to place the agent for you? (y/n): ";
    std::cin >> generate_agent;

    if (generate_agent == 'y')
    {
        // generate a random (x,y) location until said location is vacant
        do
        {
            start_x = rand() % world.size();
            start_y = rand() % world.size();
        } while (world[start_y][start_x] != 'V');

        std::cout << "Your agent will start at (" << start_x << ", " << start_y << ").\n";
    }
    else
    {
        do
        {
            std::cout << "Enter the agent's start location (x y): ";
            std::cin >> start_x >> start_y;
        } while (start_x < 0 || start_x >= world.size() || start_y < 0 || start_y >= world.size() || world[start_y][start_x] != 'V');
    }

    world[start_y][start_x] = 'S';
    return;
}

bool readFile(std::vector<std::vector<char>>& world, const std::string& filename)
{
    std::ifstream in(filename.c_str());

    if (!in.is_open())
        return false;

    int size;
    in >> size >> size;

    world.resize(size);
    for (int i = 0; i < size; i++)
        world[i].resize(size);

    for (int i = world.size() - 1; i >= 0; i--)
        for (int j = 0; j < world.size(); j++)
            in >> world[i][j];

    in.close();
    return true;
}

void writeWorldToFile(const std::vector<std::vector<char>>& world)
{
    std::ofstream out(outfile);
    out << "World\n\n" << world.size() << " " << world.size() << "\n";

    for (int y = world.size() - 1; y >= 0; y--)
    {
        for (int x = 0; x < world.size(); x++)
            out << world[y][x] << " ";

        out << "\n";
    }

    out << "\n";
    out.close();
    return;
}

void print(const std::vector<std::vector<char>>& world)
{
    for (int y = world.size() - 1; y >= 0; y--)
    {
        for (int x = 0; x < world.size(); x++)
            std::cout << world[y][x] << " ";

        std::cout << "\n";
    }

    std::cout << "\n";
    return;
}

void write(std::vector<std::vector<char>>& policy, const std::vector<std::vector<char>>& world, const std::string& label)
{
    std::ofstream out(outfile, std::fstream::app);
    out << label << "\n\n";
    for (int y = world.size() - 1; y >= 0; y--)
    {
        for (int x = 0; x < world.size(); x++)
            out << (world[y][x] == 'O' ? '-' : policy[y][x]) << " ";

        out << "\n";
    }

    out << "\n";
    return;
}

void write(const std::vector<std::vector<double>>& utilities, const std::vector<std::vector<char>>& world, const std::string& label)
{
    std::ofstream out(outfile, std::fstream::app);
    out << label << "\n\n";
    for (int y = utilities.size() - 1; y >= 0; y--)
    {
        for (int x = 0; x < utilities.size(); x++)
        {
            if (world[y][x] == 'O')
                out << "----\t";
            else
                out << std::fixed << std::setprecision(2) << utilities[y][x] << "\t";
        }

        out << "\n";
    }

    out << "\n";
    return;
}
