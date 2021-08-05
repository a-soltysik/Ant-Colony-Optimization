#include "functions.h"

#include <iostream>

void readPoints(std::ifstream& read, std::vector<std::vector<Edge>>& edges)
{
    skip_lines(read, 2);
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

std::string time_to_string(uint64_t microseconds)
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

std::string path_to_string(const std::vector<size_t>& path)
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

double probability(const Ant& ant, size_t r, size_t s, std::vector<std::vector<Edge>>& edges)
{

    double numerator = edges[r][s].getNumerator();
    double denominator = 0;
    for (size_t i = 0; i < CITIES_NUMBER; i++)
    {
        if (!ant.isVisited(i))
            denominator += edges[r][i].getNumerator();
    }
    return numerator / denominator;
}

size_t position(const Ant& ant, size_t r, std::vector<std::vector<Edge>>& edges)
{
    double q = (double) (rand() % 10001) / 10000;
    double max = 0;
    size_t edge_max;
    if (q <= Q0)
    {
        for (size_t i = 0; i < CITIES_NUMBER; i++)
        {
            if (r != i && edges[r][i].getNumerator() > max && !ant.isVisited(i))
            {
                max = edges[r][i].getNumerator();
                edge_max = i;
            }
        }
        if (max != 0)
        {
            return edge_max;
        }
    } else
    {
        double sum = 0;
        size_t last;
        q = (double) (rand() % 10001) / 10000;
        for (size_t i = 0; i < CITIES_NUMBER; i++)
        {
            if (!ant.isVisited(i))
            {
                sum += probability(ant, r, i, edges);
                last = i;
                if (q <= sum)
                    return i;
            }
        }
        return last;
    }
    return 0;
}

bool enoughFiles(uint32_t amount)
{
    std::fstream file;
    for (uint32_t i = 1; i <= amount; i++)
    {
        file.open("../in/in" + std::to_string(i) + ".ant");
        if (!file.good())
            return false;
        file.close();
    }
    return true;
}

void skip_lines(std::ifstream& stream, std::size_t lines)
{
    std::string s;
    for (; lines; --lines)
        std::getline(stream, s);
}

void setParameters(std::ifstream& read)
{
    skip_lines(read, 1);
    read >> ANTS_NUMBER;
    read.ignore();
    skip_lines(read, 1);
    read >> CITIES_NUMBER;
    read.ignore();
    skip_lines(read, 1);
    read >> ITERATION_NUMBER;
    read.ignore();
    skip_lines(read, 1);
    read >> TAU0;
    read.ignore();
    skip_lines(read, 1);
    read >> ALPHA;
    read.ignore();
    skip_lines(read, 1);
    read >> BETA;
    read.ignore();
    skip_lines(read, 1);
    read >> Q0;
    read.ignore();
    skip_lines(read, 1);
    read >> RHO;
    read.ignore();
    skip_lines(read, 1);
}

void get_user_input(uint32_t& experiments, bool& numbersOnly, bool& globalUpdate, bool& localUpdate)
{
    char writeMode;

    do
    {
        std::cout << "Give the number of experiments:";
        std::cin >> experiments;
    } while (!enoughFiles(experiments));

    std::cout << "Write only distances of paths in the output file (y/n):";
    std::cin >> writeMode;

    numbersOnly = (tolower(writeMode) == 'y');

    std::cout << "Do yoy want to add local actualization? (y/n):";
    std::cin >> writeMode;

    localUpdate = (tolower(writeMode) == 'y');

    std::cout << "Do you want to add global actualization? (y/n):";
    std::cin >> writeMode;

    globalUpdate = (tolower(writeMode) == 'y');
}