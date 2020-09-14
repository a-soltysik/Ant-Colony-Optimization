#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <cctype>
#include <vector>

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

    std::vector<int> shortestPath;

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

        std::vector<Ant> ants(ANTS_NUMBER);

        int index = 0;
        for (auto &ant : ants)
        {
            ant.index() = index;
            index++;
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
                    int new_position = position(ant, ant.position(), edges);

                    if (localUpdate_)
                    {
                        edges[ant.position()][new_position].passed();
                        edges[new_position][ant.position()].passed();
                    }

                    if (globalUpdate_)
                    {
                        edges[new_position][ant.position()].ant_passed(ant);
                        edges[ant.position()][new_position].ant_passed(ant);
                    }

                    ant.extend_path(edges[ant.position()][new_position].length());
                    ant.set_visited(new_position);
                    ant.position() = new_position;

                    if (ant.has_ended())
                    {
                        if (!numbersOnly_)
                        {
                            write << "The path of the ant " << ant.index() << ": [";
                            write << path_to_string(ant.city_order()) << ". ";
                            write << "The length of the path: " << ant.path_length() << "\n";
                        }
                        else if (numbersOnly_)
                            write << ant.path_length() << "\n";
                        if (ant.path_length() < min)
                        {
                            min = ant.path_length();
                            shortestPath = ant.city_order();
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
                history[iteration] += ant.path_length();
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
        for (auto &ant : ants)
        {
            ant.erase();
        }

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
        ant.position() = rand() % CITIES_NUMBER;
        ant.set_visited(ant.position());
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
