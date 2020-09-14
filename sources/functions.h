#ifndef MROWKI_FUNCTIONS_H
#define MROWKI_FUNCTIONS_H

#include <istream>
#include "Edge.h"

Edge** read_points(std::istream& read, Edge **edges);

std::string time_to_string(long microseconds);

std::string path_to_string(std::vector<int> path);

double distance(double x1, double y1, double x2, double y2);

double probability(Ant ant, int r, int s, Edge** edges);

int position(Ant ant, int r, Edge** edges);

bool enoughFiles(int amount);

void skip_lines(std::istream& stream, std::size_t lines);

void setParameters(std::istream& read);

void get_user_input(int &experiments, bool &numbersOnly, bool &globalUpdate, bool &localUpdate);

#endif //MROWKI_FUNCTIONS_H