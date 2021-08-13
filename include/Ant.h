#pragma once

#include "parameters.h"

#include <vector>

class Ant
{
private:
    size_t position = 0;
    const size_t ant_index;
    double path_length = 0;
    std::vector<bool> is_visited;
    std::vector<size_t> city_order;

    void visit(size_t position);
public:
    Ant(size_t index);
    Ant(Ant&&) = default;
    Ant(const Ant&) = default;
    Ant& operator=(const Ant& ant) = delete;
    ~Ant() = default;

    std::vector<size_t> getCityOrder() const { return city_order; }
    void extendPath(double length) { path_length += length; };
    double getPathLength() const { return path_length; };
    size_t getPosition() const { return position; }
    void setPosition(size_t position);
    size_t getIndex() const { return ant_index; }
    bool isVisited(size_t position) const { return is_visited[position]; }
    bool hasEnded();
    void clear();
};