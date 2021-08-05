#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <cctype>
#include <vector>
#include <string>

#include "Edge.h"
#include "Ant.h"
#include "functions.h"
#include "constants.h"

void resetAnts(std::vector<Ant> &ants);

void resetEdges(Edge **edges);

void localUpdate(Edge **edges);

void globalUpdate(Edge **edges);

int main()
{

    std::vector<uint32_t> shortestPath;

    std::ifstream read;
    std::ofstream write;

    Edge **edges;

    double eta;
    double *history;

    int experiments;
    bool numbersOnly_, globalUpdate_, localUpdate_;

    get_user_input(experiments, numbersOnly_, globalUpdate_, localUpdate_);

    auto start = std::chrono::high_resolution_clock::now();

    for (int expetiment = 0; expetiment < experiments; ++expetiment)
    {
        //reading from the input file
        read.open("../in/in" + std::to_string(expetiment + 1) + ".ant");

        setParameters(read);

        edges = new Edge *[CITIES_NUMBER];
        for (int i = 0; i < CITIES_NUMBER; i++)
        {
            edges[i] = new Edge[CITIES_NUMBER];
        }

        edges = read_points(read, edges);

        for (int i = 0; i < CITIES_NUMBER; i++)
        {
            for (int j = 0; j < CITIES_NUMBER; j++)
            {
                edges[i][j].pheromone() = TAU0;
                eta = (double)1 / edges[i][j].length();
                edges[i][j].eta() = eta;
            }
        }

        std::vector<Ant> ants;
        ants.reserve(ANTS_NUMBER);

        for (size_t i = 0; i < ANTS_NUMBER; i++) {
            ants.emplace_back(i);
        }
        history = new double[ITERATION_NUMBER];

        for (int i = 0; i < ITERATION_NUMBER; ++i)
        {
            history[i] = 0;
        }

        read.close();

        //Writing the output to the output file
        write.open("../out/out" + std::to_string(expetiment + 1) + ".ant", std::ofstream::out | std::ofstream::trunc);

        double min = 1000000000;

        for (int iteration = 0; iteration < ITERATION_NUMBER; ++iteration)
        {
            resetAnts(ants);

            resetEdges(edges);

            //main loop
            for (int step = 0; step < CITIES_NUMBER - 1; ++step)
            {
                //Resetting the number of ants on edges
                for (int i = 0; i < CITIES_NUMBER; ++i)
                {
                    for (int j = 0; j < CITIES_NUMBER; ++j)
                    {
                        edges[i][j].clear_passed();
                    }
                }
                for (auto &ant : ants)
                {
                    //setting new positions for ants
                    size_t new_position = position(ant, ant.getPosition(), edges);

                    if (localUpdate_)
                    {
                        edges[ant.getPosition()][new_position].passed();
                        edges[new_position][ant.getPosition()].passed();
                    }

                    if (globalUpdate_)
                    {
                        edges[new_position][ant.getPosition()].ant_passed(ant);
                        edges[ant.getPosition()][new_position].ant_passed(ant);
                    }

                    ant.extendPath(edges[ant.getPosition()][new_position].length());
                    ant.setPosition(new_position);

                    if (ant.has_ended())
                    {
                        if (!numbersOnly_)
                        {
                            write << "The path of the ant " << ant.getIndex() << ": [";
                            write << path_to_string(ant.getCityOrder()) << ". ";
                            write << "The length of the path: " << ant.getPathLength() << "\n";
                        }
                        else if (numbersOnly_)
                            write << ant.getPathLength() << "\n";
                        if (ant.getPathLength() < min)
                        {
                            min = ant.getPathLength();
                            shortestPath = ant.getCityOrder();
                        }
                    }
                }
                if (localUpdate_)
                {
                    localUpdate(edges);
                }
            }
            if (globalUpdate_)
            {
                globalUpdate(edges);
            }

            //saving paths of ants
            for (auto &ant : ants)
            {
                history[iteration] += ant.getPathLength();
            }
            history[iteration] /= ANTS_NUMBER;
        }

        write << "\nThe length of the shortest path: " << min << "\n";
        write << "The shortest path: " << path_to_string(shortestPath) << "\n";

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        write << "Time: " << time_to_string(duration.count());
        write.close();

        //Clearing memory

        for (int i = 0; i < CITIES_NUMBER; ++i)
        {
            delete[] edges[i];
        }

        delete[] edges;

        delete[] history;
    }

    return 0;
}

void resetAnts(std::vector<Ant> &ants)
{
    for (auto &ant : ants)
    {
        ant.clear();
        ant.setPosition(rand() % CITIES_NUMBER);
    }
}

void resetEdges(Edge **edges)
{
    for (int i = 0; i < CITIES_NUMBER; i++)
    {
        for (int j = 0; j < CITIES_NUMBER; j++)
        {
            edges[i][j].clear();
        }
    }
}

void localUpdate(Edge **edges)
{
    for (int i = 0; i < CITIES_NUMBER; ++i)
    {
        for (int j = 0; j < CITIES_NUMBER; ++j)
        {
            if (edges[i][j].pheromone() > pow(10, -100))
                edges[i][j].pheromone_update();
        }
    }
}

void globalUpdate(Edge **edges)
{
    for (int i = 0; i < CITIES_NUMBER; ++i)
    {
        for (int j = 0; j < CITIES_NUMBER; ++j)
        {
            if (edges[i][j].pheromone() > pow(10, -100))
                edges[i][j].global_pheromone_update();
        }
    }
}
