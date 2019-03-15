#pragma once

#include <string>
#include <vector>

// enum Direction {North, East, South, West};
enum PrintValue {World, Utility, Policy};

// each cell in the gridworld has a letter representation, a best move, and a utility value
struct State
{
    char name;
    char move;
    double utility;
};

// allows for simple storage of four utility values for a given state
struct ActionValues
{
    double north;
    double south;
    double east;
    double west;
};

// computes the utility of each state and the overall move policy using value iteration
void valueIteration(std::vector<std::vector<char>>& world);

// computes the utility of each state and the overall move policy using TD(0)/SARSA
void tdZero(std::vector<std::vector<char>>& world, const int& start_x, const int& start_y);

// computes the utility of each state and the overall move policy using TD(λ)
void tdLambda(std::vector<std::vector<char>>& world, const int& start_x, const int& start_y);

// evaluate how good the policy actually is
std::pair<int, int> evaluatePolicy(const std::vector<std::vector<char>>& world,
    const std::vector<std::vector<char>>& policy, const int& start_x, const int& start_y);

// this updates a's four utilities for a given state
ActionValues evaluateActionValues(const int& x, const int& y,
    const std::vector<std::vector<double>>& utilities, const std::vector<std::vector<char>>& world);

// this finds the max action/utility pair for a given state
std::pair<char, double> evaluateBestAction(const ActionValues& a);

// make the given move and return the new location
std::pair<int, int> makeMove(const char& direction, int x, int y, const std::vector<std::vector<char>>& world);

// return the (x,y) result of moving <direction> in the given state
std::pair<int,int> northCell(const int& x, const int& y, const std::vector<std::vector<char>>& world);
std::pair<int,int> southCell(const int& x, const int& y, const std::vector<std::vector<char>>& world);
std::pair<int,int> eastCell(const int& x, const int& y, const std::vector<std::vector<char>>& world);
std::pair<int,int> westCell(const int& x, const int& y, const std::vector<std::vector<char>>& world);

// this gives each of our move, utility, and new_utility vectors a clean slate
void initializeTables(std::vector<std::vector<char>>& moves,
    std::vector<std::vector<double>>& utilities,const std::vector<std::vector<char>>& world);

// this calls generateWorld() or readFile(), depending on whether or not the user has a world file
bool instantiateWorld(std::vector<std::vector<char>>& world, int& start_x, int& start_y);

// this generates a world with a random goal location; each square has a 20% chance of containing an obstacle
void generateWorld(const int& size, std::vector<std::vector<char>>& world);

// this places the agent at a start square either randomly or at the user's request
void setStartLocation(std::vector<std::vector<char>>& world, int& start_x, int& start_y);

// this reads a file into a world instance
bool readFile(std::vector<std::vector<char>>& world, const std::string& filename);

// this writes a generated world to a file so that the world can be used in future executions
void writeWorldToFile(const std::vector<std::vector<char>>& world);

// this prints the world for the user's benefit
void print(const std::vector<std::vector<char>>& world);

// this writes the policy to a file for the user's benefit
void write(std::vector<std::vector<char>>& policy, const std::vector<std::vector<char>>& world, const std::string& label);

// this writes the utilities to a file for the user's benefit
void write(const std::vector<std::vector<double>>& utilities, const std::vector<std::vector<char>>& world, const std::string& label);
