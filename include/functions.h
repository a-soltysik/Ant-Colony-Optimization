#pragma once
#include <istream>
#include "Edge.h"

void readPoints(std::istream& read, std::vector<std::vector<Edge>>& edges);

std::string time_to_string(long microseconds);

std::string path_to_string(const std::vector<uint32_t>& path);

double distance(double x1, double y1, double x2, double y2);

double probability(Ant ant, int r, int s, Edge** edges);

uint32_t position(const Ant& ant, uint32_t r, std::vector<std::vector<Edge>>& edges);

bool enoughFiles(int amount);

void skip_lines(std::istream& stream, std::size_t lines);

void setParameters(std::istream& read);

void get_user_input(int &experiments, bool &numbersOnly, bool &globalUpdate, bool &localUpdate);