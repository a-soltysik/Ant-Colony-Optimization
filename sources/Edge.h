#ifndef MROWKI_EDGE_H
#define MROWKI_EDGE_H

#include "Ant.h"
#include <vector>

class Edge
{
private:
    std::vector<Ant> ants_;
    double eta_;
    double numerator_;
    double pheromone_;
    double length_;
    int already_passed_ = 0;
    bool contains(Ant ant);

    void set_numerator();

public:
    void ant_passed(Ant ant);

    double &eta();

    double &numerator();

    double &pheromone();
    const double &pheromone() const;

    void global_pheromone_update();
    void pheromone_update();

    double &length();
    const double &length() const;

    void passed();

    void clear_passed();
    void clear();
};
#endif //MROWKI_EDGE_H
