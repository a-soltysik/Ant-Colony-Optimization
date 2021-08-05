#pragma once
#include <vector>

class Ant
{
private:
    std::vector<size_t> city_order;
    double path_length = 0;
    size_t position = 0;
    const size_t ant_index;
    std::vector<bool> is_visited;

    void visit(size_t position);

public:
    Ant(size_t index);
    ~Ant() = default;
    Ant& operator=(const Ant& ant) = delete;

    std::vector<size_t> getCityOrder() const {return city_order;}

    void extendPath(double length) {path_length += length;};
    double getPathLength() const {return path_length;};

    size_t getPosition() const {return position;}
    void setPosition(size_t position);

    size_t getIndex() const {return ant_index;}
    bool isVisited(size_t position) const {return is_visited[position];}

    bool hasEnded();

    void clear();
};