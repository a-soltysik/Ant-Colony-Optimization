#include "Experiment.h"

void Experiment::run() {
    fout.open("/out" + std::to_string(index) + ".out");
    start = std::chrono::high_resolution_clock::now();
    prepareEdges();
    prepareAnts();
    for (size_t i = 0; i < ITERATION_NUMBER; i++) {
        runIteration();
        if (IS_GLOBAL_UPDATE)
            globalUpdate();
    }
    writeSummary();
}

void Experiment::runIteration() {
    resetAnts();
    resetEdges();
    for (size_t step = 0; step < CITIES_NUMBER - 1; step++) {
        resetAntsNumberOnEdges();
        for (auto& ant : ants) {
            moveAnt(ant);
            if (ant.hasEnded())
                writeData(ant);
        }
        if (IS_LOCAL_UPDATE)
            localUpdate();
    }
}

void Experiment::moveAnt(Ant& ant) {
    size_t new_position = position(ant, ant.getPosition());
    if (IS_LOCAL_UPDATE)
    {
        edges[ant.getPosition()][new_position].passed();
        edges[new_position][ant.getPosition()].passed();
    }
    if (IS_GLOBAL_UPDATE)
    {
        edges[new_position][ant.getPosition()].antPassed(ant);
        edges[ant.getPosition()][new_position].antPassed(ant);
    }

    ant.extendPath(edges[ant.getPosition()][new_position].getLength());
    ant.setPosition(new_position);
}

void Experiment::writeData(const Ant& ant) {
    if (!IS_NUMBERS_ONLY)
    {
        fout << "The path of the ant " << ant.getIndex() << ": [";
        fout << Utils::antsPathToString(ant.getCityOrder()) << ". ";
        fout << "The length of the path: " << ant.getPathLength() << "\n";
    } else
        fout << ant.getPathLength() << "\n";
    if (ant.getPathLength() < shortestPathLength)
    {
        shortestPathLength = ant.getPathLength();
        shortestPath = ant.getCityOrder();
    }
}

void Experiment::writeSummary() {
    fout << "\nThe length of the shortest path: " << shortestPathLength << "\n";
    fout << "The shortest path: " << Utils::antsPathToString(shortestPath) << "\n";

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    fout << "Time: " << Utils::timeToString(duration.count());
    fout.close();
}

void Experiment::localUpdate() {
    for (auto& line : edges)
    {
        for (auto& edge : line)
        {
            if (edge.getPheromone() > pow(10, -100))
                edge.pheromoneUpdate();
        }
    }
}

void Experiment::globalUpdate() {
    for (auto& line : edges)
    {
        for (auto& edge : line)
        {
            if (edge.getPheromone() > pow(10, -100))
                edge.globalPheromoneUpdate();
        }
    }
}

void Experiment::prepareEdges() {
    for (auto& line : edges)
    {
        for (auto& edge : line)
        {
            edge.setPheromone(TAU0);
            edge.setEta(1 / static_cast<double>(edge.getLength()));
        }
    }
}

void Experiment::prepareAnts() {
    ants.reserve(ANTS_NUMBER);
    for (size_t i = 0; i < ANTS_NUMBER; i++)
    {
        ants.emplace_back(i);
    }
}

void Experiment::resetEdges() {
    for (auto& line : edges)
    {
        for (auto& edge : line)
        {
            if (edge.getPheromone() > pow(10, -100))
                edge.clear();
        }
    }
}

void Experiment::resetAnts() {
    for (auto& ant : ants)
    {
        ant.clear();
        ant.setPosition(Utils::getRandomUInt(0, CITIES_NUMBER - 1));
    }
}

void Experiment::resetAntsNumberOnEdges() {
    for (auto& line : edges)
    {
        for (auto& edge : line)
        {
            edge.clearPassed();
        }
    }
}

double Experiment::probability(const Ant& ant, size_t currentPos, size_t futurePos)
{
    double numerator = edges[currentPos][futurePos].getNumerator();
    double denominator = 0;
    for (size_t i = 0; i < CITIES_NUMBER; i++)
    {
        if (!ant.isVisited(i))
            denominator += edges[currentPos][i].getNumerator();
    }
    return numerator / denominator;
}

size_t Experiment::position(const Ant& ant, size_t currentPos)
{
    const double randomCoef = Utils::getRandom0to1();
    if (randomCoef <= Q0)
        return findLocallyBestEdge(ant, currentPos);
    else
        return findBestEdgeBasedOnProbability(ant, currentPos);
}

size_t Experiment::findLocallyBestEdge(const Ant& ant,size_t currentPos)
{
    double maxValue = 0;
    size_t bestEdge;
    for (size_t i = 0; i < CITIES_NUMBER; i++)
    {
        if (currentPos != i
            && edges[currentPos][i].getNumerator() > maxValue
            && !ant.isVisited(i))
        {
            maxValue = edges[currentPos][i].getNumerator();
            bestEdge = i;
        }
    }
    return bestEdge;
}

size_t Experiment::findBestEdgeBasedOnProbability(const Ant& ant,size_t currentPos)
{
    double sum = 0;
    const double randomCoef = Utils::getRandom0to1();
    size_t last;
    for (size_t i = 0; i < CITIES_NUMBER; i++)
    {
        if (!ant.isVisited(i))
        {
            sum += probability(ant, currentPos, i);
            last = i;
            if (randomCoef <= sum)
                return i;
        }
    }
    return last;
}


