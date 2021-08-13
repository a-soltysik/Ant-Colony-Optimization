#include "Ant.h"

Ant::Ant(size_t index) : ant_index(index)
{
    is_visited.reserve(CITIES_NUMBER);
    city_order.reserve(CITIES_NUMBER);
}

void Ant::setPosition(uint32_t position)
{
    this->position = position;
    visit(position);
}

void Ant::visit(uint32_t position)
{
    is_visited[position] = true;
    city_order.push_back(position);
}

bool Ant::hasEnded()
{
    for (size_t i = 0; i < CITIES_NUMBER; ++i)
    {
        if (!is_visited[i])
            return false;
    }
    return true;
}

void Ant::clear()
{
    for (size_t i = 0; i < CITIES_NUMBER; ++i)
    {
        is_visited[i] = false;
    }
    city_order.clear();
    path_length = 0;
}