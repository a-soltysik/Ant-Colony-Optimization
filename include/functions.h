#pragma once
#include "Edge.h"

#include <fstream>

void readPoints(std::ifstream& read, std::vector<std::vector<Edge>>& edges);

std::string time_to_string(uint64_t microseconds);

std::string path_to_string(const std::vector<size_t>& path);

double distance(double x1, double y1, double x2, double y2);

double probability(Ant ant, size_t r, size_t s, Edge** edges);

size_t position(const Ant& ant, size_t r, std::vector<std::vector<Edge>>& edges);

bool enoughFiles(uint32_t amount);

void skip_lines(std::ifstream& stream, std::size_t lines);

void setParameters(std::ifstream& read);

void get_user_input(uint32_t &experiments, bool &numbersOnly, bool &globalUpdate, bool &localUpdate);