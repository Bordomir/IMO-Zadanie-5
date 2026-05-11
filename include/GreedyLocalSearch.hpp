#pragma once

#include <vector>
#include <string>
#include <optional>
#include <random>
#include <memory>

#include "LocalSearch.hpp"
#include "DataLoader.hpp"
#include "Solver.hpp"

using namespace std;

class GreedyLocalSearch : public LocalSearch
{
public:
    mt19937 rng;
    vector<int> indices;
    vector<int> solutionIndices1;
    vector<int> solutionIndices2;
    vector<bool> isChangingNodes;
    vector<int> inSolution;

    GreedyLocalSearch(unique_ptr<Solver> &solver, MoveType neighbourhood, unsigned int seed = random_device{}()) : LocalSearch(solver, neighbourhood), rng(seed) {};
    GreedyLocalSearch(DataLoader &data, vector<int> solution, MoveType neighbourhood, unsigned int seed = random_device{}()) : LocalSearch(data, solution, neighbourhood), rng(seed) {};
    string getAlgorithmName() override;
    void setMoveSet() override;
    optional<Move> chooseMove() override;
    void updateMoveSet(const Move &move) override;
};