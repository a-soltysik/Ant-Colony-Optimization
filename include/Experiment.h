#pragma once
#include "Utils.h"

#include <chrono>
#include <utility>

class Experiment
{
private:
    std::vector<std::vector<Edge>> edges;
    std::vector<Ant> ants;
    std::ofstream fout;
    double shortestPathLength = std::numeric_limits<double>::max();
    std::vector<size_t> shortestPath;
    uint32_t index;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;

    void prepareEdges();
    void prepareAnts();
    void resetEdges();
    void resetAntsNumberOnEdges();
    void resetAnts();
    void runIteration();
    void moveAnt(Ant& ant);
    void writeData(const Ant& ant);
    void writeSummary();
    void localUpdate();
    void globalUpdate();
    double probability(const Ant& ant, size_t currentPos, size_t futurePos);
    size_t position(const Ant& ant, size_t currentPos);
    size_t findLocallyBestEdge(const Ant& ant, size_t currentPos);
    size_t findBestEdgeBasedOnProbability(const Ant& ant, size_t currentPos);

public:
    Experiment(uint32_t index, std::vector<std::vector<Edge>>  edges)
            : index(index),
            edges(std::move(edges)){}

    Experiment(Experiment&&) = default;
    Experiment(const Experiment&) = delete;
    Experiment& operator=(const Experiment&) = delete;
    ~Experiment() = default;

    void run();


};
