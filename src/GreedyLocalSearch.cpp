#include "../include/GreedyLocalSearch.hpp"

#include <string>
#include <format>
#include <optional>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

string GreedyLocalSearch::getAlgorithmName()
{
    return format("Greedy_{}", neighbourhoodUsed == MoveType::SwapNodes ? "SwapNodes" : "SwapEdges");
}

void GreedyLocalSearch::setMoveSet()
{ 
    moveSet.clear();
    int n = solution.size();

    indices = vector<int>(data->numNodes);
    solutionIndices1 = vector<int>(n);
    solutionIndices2 = vector<int>(n);
    iota(indices.begin(), indices.end(), 0);
    iota(solutionIndices1.begin(), solutionIndices1.end(), 0);
    iota(solutionIndices2.begin(), solutionIndices2.end(), 0);
    
    inSolution = vector<int>(data->numNodes, -1);
    for (size_t i = 0; i < solution.size(); i++)
        inSolution[solution[i]] = i;

    isChangingNodes = {false, true};
}

optional<Move> GreedyLocalSearch::chooseMove()
{
    shuffle(indices.begin(), indices.end(), rng);
    shuffle(solutionIndices1.begin(), solutionIndices1.end(), rng);
    shuffle(solutionIndices2.begin(), solutionIndices2.end(), rng);
    shuffle(isChangingNodes.begin(), isChangingNodes.end(), rng);

    size_t indicesIndex = 0;
    bool hasTriedToRemove;
    optional<MoveType> moveType;
    int node1, node2;
    for(int i : solutionIndices1)
    {
        hasTriedToRemove = false;
        for (int j : solutionIndices2)
        {
            for (bool isChanging : isChangingNodes)
            {
                moveType = nullopt;
                if(isChanging && !hasTriedToRemove)
                {
                    node1 = indices[indicesIndex];
                    if(inSolution[node1] >= 0)
                    {
                        hasTriedToRemove = true;
                        moveType = MoveType::RemoveNode;
                        node2 = inSolution[node1];
                    } else
                    {
                        moveType = MoveType::InsertNode;
                        node2 = j;
                    }

                }else
                {
                    if(i < j)
                    {
                        moveType = neighbourhoodUsed;
                        node1 = i;
                        node2 = j;
                    }
                }

                if(moveType)
                {
                    Move move(*moveType, node1, node2);
                    move.deltaScore = calculateDeltaScore(move);
                    if(*move.deltaScore > 0)
                    {
                        return move;
                    }
                }
            }
        }
        indicesIndex++;
    }
    while(indicesIndex < indices.size())
    {
        node1 = indices[indicesIndex];
        if(inSolution[node1] >= 0)
        {
            Move move(MoveType::RemoveNode, node1, inSolution[node1]);
            move.deltaScore = calculateDeltaScore(move);
            if(*move.deltaScore > 0)
            {
                return move;
            }
        } else
        {
            for (int j : solutionIndices2)
            {
                Move move(MoveType::InsertNode, node1, j);
                move.deltaScore = calculateDeltaScore(move);
                if(*move.deltaScore > 0)
                {
                    return move;
                }
            }
        }
        indicesIndex++;
    }
    return nullopt;
}

void GreedyLocalSearch::updateMoveSet(const Move &move)
{
    const int n = solution.size();
    switch (move.type)
    {
        case MoveType::InsertNode:
        {
            solutionIndices1.push_back(n-1);
            solutionIndices2.push_back(n-1);

            int newNodeSolutionIndex = *move.node2 + 1;
            inSolution[move.node1] = newNodeSolutionIndex++;
            for (; newNodeSolutionIndex < n; newNodeSolutionIndex++)
                inSolution[solution[newNodeSolutionIndex]]++;
            break;
        }
        case MoveType::RemoveNode:
        {
            solutionIndices1.resize(n);
            solutionIndices2.resize(n);
            iota(solutionIndices1.begin(), solutionIndices1.end(), 0);
            iota(solutionIndices2.begin(), solutionIndices2.end(), 0);

            int removedNodeSolutionIndex = *move.node2;
            inSolution[move.node1] = -1;
            for (; removedNodeSolutionIndex < n; removedNodeSolutionIndex++)
                inSolution[solution[removedNodeSolutionIndex]]--;
            break;
        }
        case MoveType::SwapNodes:
        {
            inSolution[solution[move.node1]] = move.node1;
            inSolution[solution[*move.node2]] = *move.node2;
            break;
        }
        case MoveType::SwapEdges:
        {
            for (int i = move.node1 + 1; i <= *move.node2; i++)
            {
                inSolution[solution[i]] = i;
            }
            break;
        }
    }
}
