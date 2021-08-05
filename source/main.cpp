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

void resetAnts(std::vector<Ant>& ants);

void resetEdges(std::vector<std::vector<Edge>>& edges);

void localUpdate(std::vector<std::vector<Edge>>& edges);

void globalUpdate(std::vector<std::vector<Edge>>& edges);

int main() {

    std::vector<uint32_t> shortestPath;

    std::ifstream read;
    std::ofstream write;

    std::vector<std::vector<Edge>> edges(
            CITIES_NUMBER,
            std::vector<Edge>(CITIES_NUMBER, Edge())
    );

    double eta;
    double* history;

    int experiments;
    bool numbersOnly_, globalUpdate_, localUpdate_;

    get_user_input(experiments, numbersOnly_, globalUpdate_, localUpdate_);

    auto start = std::chrono::high_resolution_clock::now();

    for (int expetiment = 0; expetiment < experiments; ++expetiment) {
        //reading from the input file
        read.open("../in/in" + std::to_string(expetiment + 1) + ".ant");

        setParameters(read);

        readPoints(read, edges);

        for (auto& line : edges) {
            for (auto& edge : line) {
                edge.setPheromone(TAU0);
                eta = 1 / static_cast<double>(edge.getLength());
                edge.setEta(eta);
            }
        }

        std::vector<Ant> ants;
        ants.reserve(ANTS_NUMBER);

        for (size_t i = 0; i < ANTS_NUMBER; i++) {
            ants.emplace_back(i);
        }
        history = new double[ITERATION_NUMBER];

        for (int i = 0; i < ITERATION_NUMBER; ++i) {
            history[i] = 0;
        }

        read.close();

        //Writing the output to the output file
        write.open("../out/out" + std::to_string(expetiment + 1) + ".ant", std::ofstream::out | std::ofstream::trunc);

        double min = 1000000000;

        for (int iteration = 0; iteration < ITERATION_NUMBER; ++iteration) {
            resetAnts(ants);

            resetEdges(edges);

            //main loop
            for (int step = 0; step < CITIES_NUMBER - 1; ++step) {
                //Resetting the number of ants on edges
                for (int i = 0; i < CITIES_NUMBER; ++i) {
                    for (int j = 0; j < CITIES_NUMBER; ++j) {
                        edges[i][j].clearPassed();
                    }
                }
                for (auto& ant : ants) {
                    //setting new positions for ants
                    size_t new_position = position(ant, ant.getPosition(), edges);

                    if (localUpdate_) {
                        edges[ant.getPosition()][new_position].passed();
                        edges[new_position][ant.getPosition()].passed();
                    }

                    if (globalUpdate_) {
                        edges[new_position][ant.getPosition()].antPassed(ant);
                        edges[ant.getPosition()][new_position].antPassed(ant);
                    }

                    ant.extendPath(edges[ant.getPosition()][new_position].getLength());
                    ant.setPosition(new_position);

                    if (ant.hasEnded()) {
                        if (!numbersOnly_) {
                            write << "The path of the ant " << ant.getIndex() << ": [";
                            write << path_to_string(ant.getCityOrder()) << ". ";
                            write << "The length of the path: " << ant.getPathLength() << "\n";
                        } else if (numbersOnly_)
                            write << ant.getPathLength() << "\n";
                        if (ant.getPathLength() < min) {
                            min = ant.getPathLength();
                            shortestPath = ant.getCityOrder();
                        }
                    }
                }
                if (localUpdate_) {
                    localUpdate(edges);
                }
            }
            if (globalUpdate_) {
                globalUpdate(edges);
            }

            //saving paths of ants
            for (auto& ant : ants) {
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

        delete[] history;
    }

    return 0;
}

void resetAnts(std::vector<Ant>& ants) {
    for (auto& ant : ants) {
        ant.clear();
        ant.setPosition(rand() % CITIES_NUMBER);
    }
}

void resetEdges(std::vector<std::vector<Edge>>& edges) {
    for (auto& line : edges) {
        for (auto& edge : line) {
            if (edge.getPheromone() > pow(10, -100))
                edge.clear();
        }
    }
}

void localUpdate(std::vector<std::vector<Edge>>& edges) {
    for (auto& line : edges) {
        for (auto& edge : line) {
            if (edge.getPheromone() > pow(10, -100))
                edge.pheromoneUpdate();
        }
    }
}

void globalUpdate(std::vector<std::vector<Edge>>& edges) {
    for (auto& line : edges) {
        for (auto& edge : line) {
            if (edge.getPheromone() > pow(10, -100))
                edge.globalPheromoneUpdate();
        }
    }
}
