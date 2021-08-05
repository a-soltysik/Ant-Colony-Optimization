#include "Edge.h"

#include <algorithm>

void Edge::antPassed(const Ant& ant) {
    if (!contains(ant)) {
        ants.push_back(ant);
    }
}

double Edge::getNumerator() {
    set_numerator();
    return numerator;
}

void Edge::globalPheromoneUpdate() {
    double sum = 0;
    for (Ant& ant : ants) {
        if (ant.getPathLength() > 0)
            sum += (double) 1 / ant.getPathLength();
    }
    pheromone = (1 - ALPHA) * pheromone + sum;
}

void Edge::clear() {
    clearPassed();
    ants.clear();
}

bool Edge::contains(const Ant& ant) {
    return std::any_of(
            ants.begin(),
            ants.end(),
            [&ant](const Ant& vecAnt) {
                return ant.getIndex() == vecAnt.getIndex();
            }
    );
}