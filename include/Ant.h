#ifndef MROWKI_ANT_H
#define MROWKI_ANT_H

#include <vector>

class Ant
{
private:
    std::vector<int> city_order_;
    double path_ = 0;
    int position_;
    int index_;
    bool *is_visited_;

public:
    Ant();

    int city_order(int index);
    const std::vector<int> &city_order() const;

    void extend_path(double length);
    double path_length();

    int &position();
    const int &position() const;

    int &index();
    const int &index() const;

    void set_visited(int position);
    bool is_visited(int position);

    bool has_ended();

    void clear();

    void erase();
};

#endif //MROWKI_ANT_H
