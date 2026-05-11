#include <string>
#include <vector>
#include <memory>
#include <print>
#include <random>
#include <limits>
#include <ranges>
#include <algorithm>
#include <utility>
#include <chrono>

#include "../include/DataLoader.hpp"
#include "../include/Solver.hpp"
#include "../include/RandomSolver.hpp"
#include "../include/LocalSearch.hpp"
#include "../include/GreedyLocalSearch.hpp"

using namespace std;

struct Statistic
{
    string data;
    string solver;
    string localSearch;
    double average = 0;
    double min = numeric_limits<double>::max();
    double max = numeric_limits<double>::min();

    Statistic(string data, string solver, string localSearch) : data(move(data)), solver(move(solver)), localSearch(move(localSearch)) {};
    void update(double value)
    {
        average += value;
        min = std::min(min, value);
        max = std::max(max, value);
    }
    void print() const
    {
        std::println("{};{};{};{:.4f};{:.4f};{:.4f}", data, solver, localSearch, average, min, max);
    }
};

int main()
{
    DataLoader dataA("../data/TSPA.csv", "DataA");
    DataLoader dataB("../data/TSPB.csv", "DataB");

    int startNode = 0;

    RandomSolver randomSolverA(dataA, 0);
    RandomSolver randomSolverB(dataB, 0);

    GreedyLocalSearch greedyLocalSearchA(dataA, randomSolverA.solution, MoveType::SwapEdges);
    GreedyLocalSearch greedyLocalSearchB(dataB, randomSolverB.solution, MoveType::SwapEdges);

    // Experiment
    mt19937 rng{42};
    int numRuns = 1000;

    vector<vector<int>> solutionsA;
    vector<vector<int>> solutionsB;

    vector<int> solutionScoresA;
    vector<int> solutionScoresB;
    for (int i = 0; i < numRuns; i++)
    {
        println("Run {}", i);
        randomSolverA.solve();
        randomSolverB.solve();

        greedyLocalSearchA.solution = randomSolverA.solution;
        greedyLocalSearchB.solution = randomSolverB.solution;

        greedyLocalSearchA.improve();
        greedyLocalSearchB.improve();

        greedyLocalSearchA.saveToFile(format("DataA_{}", i));
        greedyLocalSearchB.saveToFile(format("DataB_{}", i));

        // solutionsA.push_back(greedyLocalSearchA.solution);
        // solutionsB.push_back(greedyLocalSearchB.solution);

        // solutionScoresA.push_back(greedyLocalSearchA.solutionScore);
        // solutionScoresB.push_back(greedyLocalSearchB.solutionScore);
    }
    
    
    // vector<unique_ptr<RandomLocalSearch>> randomLocalSearches;
    // randomLocalSearches.reserve(2);
    // randomLocalSearches.emplace_back(make_unique<RandomLocalSearch>(solvers[0], MoveType::SwapNodes, randomTimeLimit));
    // randomLocalSearches.emplace_back(make_unique<RandomLocalSearch>(solvers[0], MoveType::SwapEdges, randomTimeLimit));

    // vector<Statistic> scoreStatisticsForRandomLocalSearch;
    // scoreStatisticsForRandomLocalSearch.reserve(solvers.size() * randomLocalSearches.size());
    // vector<Statistic> timeStatisticsForRandomLocalSearch;
    // timeStatisticsForRandomLocalSearch.reserve(solvers.size() * randomLocalSearches.size());
    // for (auto &solver : solvers)
    // {
    //     for (auto &localSearch : randomLocalSearches)
    //     {
    //         scoreStatisticsForRandomLocalSearch.emplace_back(
    //             solver->data->getName(),
    //             solver->getAlgorithmName(),
    //             localSearch->getAlgorithmName());
    //         timeStatisticsForRandomLocalSearch.emplace_back(
    //             solver->data->getName(),
    //             solver->getAlgorithmName(),
    //             localSearch->getAlgorithmName());
    //     }
    // }
    // for (int startNode : startingNodes)
    // {
    //     for (size_t i = 0; i < solvers.size(); i++)
    //     {
    //         const auto &solver = solvers[i];

    //         solver->startNode = startNode;
    //         solver->solve();

    //         for (size_t j = 0; j < randomLocalSearches.size(); j++)
    //         {
    //             const auto &localSearch = randomLocalSearches[j];

    //             localSearch->data = solver->data;
    //             localSearch->solution = solver->solution;

    //             startTime = chrono::high_resolution_clock::now();
    //             localSearch->improve();
    //             endTime = chrono::high_resolution_clock::now();

    //             int index = i * randomLocalSearches.size() + j;
    //             if (localSearch->solutionScore > scoreStatistics[index].max)
    //             {
    //                 localSearch->saveToFile(format("{}_{}", solver->data->getName(), solver->getAlgorithmName()));
    //             }

    //             scoreStatisticsForRandomLocalSearch[index].update(localSearch->solutionScore);
    //             timeStatisticsForRandomLocalSearch[index].update(chrono::duration<double, std::milli>(endTime - startTime).count());
    //         }
    //     }
    // }
    // for(auto &stat : scoreStatisticsForRandomLocalSearch) stat.average /= maxTestsPossible;
    // for(auto &stat : timeStatisticsForRandomLocalSearch) stat.average /= maxTestsPossible;

    // auto allScoreStatistics = {
    //     scoreStatisticsForSolver,
    //     scoreStatistics,
    //     // scoreStatisticsForRandomLocalSearch
    // };

    // println("\nScore statistics:");
    // for(const auto &stat : allScoreStatistics | views::join)
    //     stat.print();

    // auto allTimeStatistics = {
    //     timeStatisticsForSolver,
    //     timeStatistics,
    //     // timeStatisticsForRandomLocalSearch
    // };

    // println("\nTime statistics:");
    // for(const auto &stat : allTimeStatistics | views::join)
    //     stat.print();

    return 0;
}