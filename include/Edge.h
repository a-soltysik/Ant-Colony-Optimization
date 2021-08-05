#pragma once

#include "Ant.h"
#include "constants.h"
#include <cmath>


class Edge {
private:
    std::vector<Ant> ants;
    double eta;
    double numerator;
    double pheromone;
    double length;
    int already_passed = 0;

    bool contains(const Ant& ant);

    void set_numerator() { numerator = pheromone * std::pow(eta, BETA); }

public:
    Edge() = default;
    ~Edge() = default;
    Edge& operator=(const Edge& edge) = delete;
    void antPassed(const Ant& ant);

    double getEta() const { return eta; }
    void setEta(double eta) {this->eta = eta;}

    double getNumerator();

    double getPheromone() const { return pheromone; }

    void setPheromone(double pheromone) { this->pheromone = pheromone; }

    void globalPheromoneUpdate();

    void pheromoneUpdate() { pheromone = (1 - RHO) * pheromone + RHO * already_passed * TAU0; }

    double getLength() const { return length; };

    void setLength(double length) { this->length = length; };

    void passed() { already_passed++; };

    void clearPassed() { already_passed = 0; };

    void clear();
};