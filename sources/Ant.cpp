#include <vector>
#include "Ant.h"
#include "constants.h"

Ant::Ant()
{
    is_visited_ = new bool[CITIES_NUMBER];
}

int Ant::city_order(int index)
{
    return city_order_[index];
}

const std::vector<int> &Ant::city_order() const 
{
    return city_order_;
}

void Ant::extend_path(double length)
{
    path_ += length;
}

double Ant::path_length()
{
    return path_;
}

int &Ant::position()
{
    return position_;
}

const int &Ant::position() const
{
    return position_;
}

int &Ant::index()
{
    return index_;
}
const int &Ant::index() const
{
    return index_;
}

void Ant::set_visited(int position)
{
    is_visited_[position] = true;
    city_order_.push_back(position);
}

bool Ant::is_visited(int position)
{
    return is_visited_[position];
}

bool Ant::has_ended()
{
    for (int i = 0; i < CITIES_NUMBER; ++i)
    {
        if (!is_visited_[i])
            return false;
    }
    return true;
}

void Ant::clear()
{
    for (int i = 0; i < CITIES_NUMBER; ++i)
    {
        is_visited_[i] = false;
    }
    city_order_.clear();
    path_ = 0;
}

void Ant::erase()
{
    delete[] is_visited_;
}