#pragma once

#include "constants.h"

#include <vector>

class Ant
{
private:
    std::size_t position = 0;
    const std::size_t ant_index;
    double path_length = 0;
    std::vector<bool> is_visited;
    std::vector<std::size_t> city_order;

    void visit(std::size_t position);
public:
    Ant(size_t index);
    ~Ant() = default;
    Ant& operator=(const Ant& ant) = delete;

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