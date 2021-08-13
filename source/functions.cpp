#include "functions.h"
#include "Utils.h"

#include <iostream>

void readPoints(std::ifstream& read, std::vector<std::vector<Edge>>& edges)
{
    skipLines(read, 2);
    std::vector<double> x(CITIES_NUMBER, 0.0);
    std::vector<double> y(CITIES_NUMBER, 0.0);
    for (size_t i = 0; i < CITIES_NUMBER; ++i)
    {
        read >> x[i] >> y[i];
        read.ignore();
    }
    for (size_t i = 0; i < CITIES_NUMBER; ++i)
        for (size_t j = 0; j < CITIES_NUMBER; ++j)
            edges[i][j].setLength(distance(x[i], y[i], x[j], y[j]));
}

std::string timeToString(uint64_t microseconds)
{
    std::cout << microseconds;
    uint16_t micros = microseconds % 1000;
    microseconds /= 1000;
    uint16_t millis = microseconds % 1000;
    microseconds /= 1000;
    uint16_t seconds = microseconds % 60;
    microseconds /= 60;
    uint16_t minutes = microseconds % 60;

    std::string time;
    if (micros > 0)
    {
        time = std::to_string(micros) + "us " + time;
    }
    if (millis > 0)
    {
        time = std::to_string(millis) + "ms " + time;
    }
    if (seconds > 0)
    {
        time = std::to_string(seconds) + "s " + time;
    }
    if (minutes > 0)
    {
        time = std::to_string(minutes) + "m " + time;
    }

    return time;
}

std::string pathToString(const std::vector<size_t>& path)
{
    std::string result = "[";
    for (size_t i = 0; i < path.size(); i++)
    {
        if (i < path.size() - 1)
        {
            result += std::to_string(path[i]) + ",";
        } else
        {
            result += std::to_string(path[i]);
        }
    }
    result += "]";
    return result;
}

double distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double probability(
        const Ant& ant,
        size_t currentPos,
        size_t futurePos,
        std::vector<std::vector<Edge>>& edges)
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

size_t position(
        const Ant& ant,
        size_t currentPos,
        std::vector<std::vector<Edge>>& edges)
{
    const double randomCoef = Utils::getRandom0to1();
    if (randomCoef <= Q0)
        return findLocallyBestEdge(ant, currentPos, edges);
    else
        return findBestEdgeBasedOnProbability(ant, currentPos, edges);
}

size_t findLocallyBestEdge(
        const Ant& ant,
        size_t currentPos,
        std::vector<std::vector<Edge>>& edges)
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

size_t findBestEdgeBasedOnProbability(
        const Ant& ant,
        size_t currentPos,
        std::vector<std::vector<Edge>>& edges)
{
    double sum = 0;
    const double randomCoef = Utils::getRandom0to1();
    size_t last;
    for (size_t i = 0; i < CITIES_NUMBER; i++)
    {
        if (!ant.isVisited(i))
        {
            sum += probability(ant, currentPos, i, edges);
            last = i;
            if (randomCoef <= sum)
                return i;
        }
    }
    return last;
}

bool enoughFiles(uint32_t amount)
{
    std::fstream file;
    for (uint32_t i = 1; i <= amount; i++)
    {
        file.open("/in" + std::to_string(i) + ".ant");
        if (!file.good())
            return false;
        file.close();
    }
    return true;
}

void skipLines(std::ifstream& stream, std::size_t lines)
{
    std::string s;
    for (; lines; --lines)
        std::getline(stream, s);
}

void setParameters(std::ifstream& read)
{
    skipLines(read, 1);
    read >> ANTS_NUMBER;
    read.ignore();
    skipLines(read, 1);
    read >> CITIES_NUMBER;
    read.ignore();
    skipLines(read, 1);
    read >> ITERATION_NUMBER;
    read.ignore();
    skipLines(read, 1);
    read >> TAU0;
    read.ignore();
    skipLines(read, 1);
    read >> ALPHA;
    read.ignore();
    skipLines(read, 1);
    read >> BETA;
    read.ignore();
    skipLines(read, 1);
    read >> Q0;
    read.ignore();
    skipLines(read, 1);
    read >> RHO;
    read.ignore();
    skipLines(read, 1);
}

void getUserInput(
        uint32_t& experiments,
        bool& numbersOnly,
        bool& globalUpdate,
        bool& localUpdate)
{
    char writeMode;

    std::cout << "Give the number of experiments:";
    std::cin >> experiments;

    if (!enoughFiles(experiments))
        throw std::invalid_argument("There are not enough *.ant files");

    std::cout << "Write only distances of paths in the output file (y/n):";
    std::cin >> writeMode;

    numbersOnly = (tolower(writeMode) == 'y');

    std::cout << "Do you want to add local actualization? (y/n):";
    std::cin >> writeMode;

    localUpdate = (tolower(writeMode) == 'y');

    std::cout << "Do you want to add global actualization? (y/n):";
    std::cin >> writeMode;

    globalUpdate = (tolower(writeMode) == 'y');
}