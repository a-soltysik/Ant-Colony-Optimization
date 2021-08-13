#include "Experiment.h"

#include <chrono>

int main()
{
    std::ifstream fin;

    Utils::getUserInput();

    for (uint32_t experimentIndex = 0; experimentIndex < EXPERIMENTS_NUMBER; experimentIndex++)
    {
        std::vector<Point> points = Utils::collectData(fin);
        std::vector<std::vector<Edge>> edges = Utils::parsePointsToEdges(points);
        Experiment(experimentIndex, edges).run();
    }
    return 0;
}