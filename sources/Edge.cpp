#include <cmath>
#include <iostream>
#include "Edge.h"
#include "Ant.h"
#include "constants.h"

void Edge::ant_passed(Ant ant)
{
    if (!contains(ant))
    {
        ants_.push_back(ant);
    }
}

double &Edge::eta()
{
    return eta_;
}

void Edge::set_numerator()
{
    numerator_ = pheromone_ * pow(eta_, BETA);
}

double &Edge::numerator()
{
    set_numerator();
    return numerator_;
}

double &Edge::pheromone()
{
    return pheromone_;
}
const double &Edge::pheromone() const
{
    return pheromone_;
}

void Edge::pheromone_update()
{
    pheromone_ = (1 - RHO) * pheromone_ + RHO * already_passed_ * TAU0;
}

void Edge::global_pheromone_update()
{
    double sum = 0;
    for (Ant &ant : ants_)
    {
        if (ant.path_length() > 0)
            sum += (double)1 / ant.path_length();
    }
    pheromone_ = (1 - ALPHA) * pheromone_ + sum;
}

double &Edge::length()
{
    return length_;
}
const double &Edge::length() const
{
    return length_;
}

void Edge::passed()
{
    already_passed_++;
}
void Edge::clear_passed()
{
    already_passed_ = 0;
}
void Edge::clear()
{
    clear_passed();
    ants_.clear();
}
bool Edge::contains(Ant ant)
{
    for (Ant &vecAnt : ants_)
    {
        if (ant.index() == vecAnt.index())
            return true;
    }
    return false;
}
