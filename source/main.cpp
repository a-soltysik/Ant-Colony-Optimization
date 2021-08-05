#include "functions.h"

#include <chrono>

void resetAnts(std::vector<Ant>& ants);

void resetEdges(std::vector<std::vector<Edge>>& edges);

void localUpdate(std::vector<std::vector<Edge>>& edges);

void globalUpdate(std::vector<std::vector<Edge>>& edges);

int main() {
    std::ifstream read;
    std::ofstream write;

    std::vector<std::vector<Edge>> edges(
            CITIES_NUMBER,
            std::vector<Edge>(CITIES_NUMBER, Edge())
    );
    std::vector<double> pathLengthsOfAll(ITERATION_NUMBER, 0.0);
    std::vector<size_t> shortestPath;

    uint32_t experimentsNumber;
    bool isNumbersOnly, isGlobalUpdate, isLocalUpdate;

    get_user_input(experimentsNumber, isNumbersOnly, isGlobalUpdate, isLocalUpdate);

    auto start = std::chrono::high_resolution_clock::now();

    for (uint32_t experimentIndex = 0; experimentIndex < experimentsNumber; experimentIndex++) {
        //reading from the input file
        read.open("../in/in" + std::to_string(experimentIndex + 1) + ".ant");

        setParameters(read);
        readPoints(read, edges);

        for (auto& line : edges) {
            for (auto& edge : line) {
                edge.setPheromone(TAU0);
                edge.setEta(1 / static_cast<double>(edge.getLength()));
            }
        }

        std::vector<Ant> ants;
        ants.reserve(ANTS_NUMBER);

        for (size_t i = 0; i < ANTS_NUMBER; i++) {
            ants.emplace_back(i);
        }

        read.close();

        //Writing the output to the output file
        write.open("../out/out" + std::to_string(experimentIndex + 1) + ".ant", std::ofstream::out | std::ofstream::trunc);

        double min = std::numeric_limits<double>::max();

        for (size_t iteration = 0; iteration < ITERATION_NUMBER; iteration++) {
            resetAnts(ants);
            resetEdges(edges);
            //main loop
            for (size_t step = 0; step < CITIES_NUMBER - 1; step++) {
                //Resetting the number of ants on edges
                for (size_t i = 0; i < CITIES_NUMBER; i++) {
                    for (size_t j = 0; j < CITIES_NUMBER; j++) {
                        edges[i][j].clearPassed();
                    }
                }
                for (auto& ant : ants) {
                    //setting new positions for ants
                    size_t new_position = position(ant, ant.getPosition(), edges);
                    if (isLocalUpdate) {
                        edges[ant.getPosition()][new_position].passed();
                        edges[new_position][ant.getPosition()].passed();
                    }
                    if (isGlobalUpdate) {
                        edges[new_position][ant.getPosition()].antPassed(ant);
                        edges[ant.getPosition()][new_position].antPassed(ant);
                    }

                    ant.extendPath(edges[ant.getPosition()][new_position].getLength());
                    ant.setPosition(new_position);

                    if (ant.hasEnded()) {
                        if (!isNumbersOnly) {
                            write << "The path of the ant " << ant.getIndex() << ": [";
                            write << path_to_string(ant.getCityOrder()) << ". ";
                            write << "The length of the path: " << ant.getPathLength() << "\n";
                        } else
                            write << ant.getPathLength() << "\n";
                        if (ant.getPathLength() < min) {
                            min = ant.getPathLength();
                            shortestPath = ant.getCityOrder();
                        }
                    }
                }
                if (isLocalUpdate) {
                    localUpdate(edges);
                }
            }
            if (isGlobalUpdate) {
                globalUpdate(edges);
            }

            //saving paths of ants
            for (auto& ant : ants) {
                pathLengthsOfAll[iteration] += ant.getPathLength();
            }
            pathLengthsOfAll[iteration] /= ANTS_NUMBER;
        }

        write << "\nThe length of the shortest path: " << min << "\n";
        write << "The shortest path: " << path_to_string(shortestPath) << "\n";

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        write << "Time: " << time_to_string(duration.count());
        write.close();
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
