#include "Experiment.h"
#include "FileException.h"

#include <chrono>

int main()
{
    try
    {
        Utils::getUserInput();
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what();
        return 1;
    }

    for (uint32_t experimentIndex = 0; experimentIndex < EXPERIMENTS_NUMBER; experimentIndex++)
    {
        std::ifstream fin("in/" + std::to_string(experimentIndex) + ".ant");
        try
        {
            std::vector<Point> points = Utils::collectData(fin);
            std::vector<std::vector<Edge>> edges = Utils::parsePointsToEdges(points);
            Experiment(experimentIndex, edges).run();
        } catch (const FileException& e) {
            std::cerr << e.what();
            return 1;
        }
    }
    return 0;
}