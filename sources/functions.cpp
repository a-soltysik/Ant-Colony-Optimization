#include <istream>
#include <cmath>
#include <fstream>
#include <iostream>
#include "Edge.h"
#include "functions.h"
#include "constants.h"

Edge **read_points(std::istream &read, Edge **edges)
{
    skip_lines(read, 2);
    double *x = new double[CITIES_NUMBER];
    double *y = new double[CITIES_NUMBER];
    for (int i = 0; i < CITIES_NUMBER; ++i)
    {
        read >> x[i] >> y[i];
        read.ignore();
    }
    for (int i = 0; i < CITIES_NUMBER; ++i)
        for (int j = 0; j < CITIES_NUMBER; ++j)
            edges[i][j].length() = distance(x[i], y[i], x[j], y[j]);

    delete[] x;
    delete[] y;
    return edges;
}

Edge **read_distances(std::istream &read, Edge **edges)
{
    double length;
    for (int i = 0; i < CITIES_NUMBER; i++)
    {
        for (int j = 0; j < CITIES_NUMBER; j++)
        {
            if (i == 0 && j == 0)
            {
                edges[i][j].length() = 0;
                continue;
            }
            read >> length;
            edges[i][j].length() = length;
        }
        read.ignore();
    }
    return edges;
}

std::string time_to_string(long microseconds)
{
    std::cout << microseconds;
    int micros = microseconds % 1000;
    microseconds /= 1000;
    int millis = microseconds % 1000;
    microseconds /= 1000;
    int seconds = microseconds % 60;
    microseconds /= 60;
    int minutes = microseconds % 60;

    std::string time = "";
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

std::string path_to_string(std::vector<int> path)
{
    std::string result = "[";
    for (int i = 0; i < path.size(); i++)
    {
        if (i < path.size() - 1)
        {
            result += std::to_string(path[i]) + ",";
        }
        else
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

double probability(Ant ant, int r, int s, Edge **edges)
{

    double numerator = edges[r][s].numerator();
    double denominator = 0;
    for (int i = 0; i < CITIES_NUMBER; ++i)
    {
        if (!ant.is_visited(i))
            denominator += edges[r][i].numerator();
    }

    return numerator / denominator;
}

int position(Ant ant, int r, Edge **edges)
{
    double q = (double)(rand() % 10001) / 10000;
    double max = 0;
    int edge_max;
    if (q <= Q0)
    {
        for (int i = 0; i < CITIES_NUMBER; ++i)
        {
            if (r != i && edges[r][i].numerator() > max && !ant.is_visited(i))
            {
                max = edges[r][i].numerator();
                edge_max = i;
            }
        }
        if (max != 0)
        {
            return edge_max;
        }
    }
    else
    {
        double sum = 0;
        int last;
        q = (double)(rand() % 10001) / 10000;
        for (int i = 0; i < CITIES_NUMBER; ++i)
        {
            if (!ant.is_visited(i))
            {
                sum += probability(ant, r, i, edges);
                last = i;
                if (q <= sum)
                    return i;
            }
        }
        return last;
    }
}

bool enoughFiles(int amount)
{
    std::fstream file;
    for (int i = 1; i <= amount; ++i)
    {
        file.open("../in/in" + std::to_string(i) + ".ant");
        if (!file.good())
            return false;
        file.close();
    }
    return true;
}

void skip_lines(std::istream &stream, std::size_t lines)
{
    std::string s;
    for (; lines; --lines)
        std::getline(stream, s);
}

void setParameters(std::istream &read)
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

void get_user_input(int &experiments, bool &numbersOnly, bool &globalUpdate, bool &localUpdate)
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