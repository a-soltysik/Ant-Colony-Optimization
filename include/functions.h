#pragma once

#include "Edge.h"

#include <fstream>

void readPoints(std::ifstream& read, std::vector<std::vector<Edge>>& edges);
std::string timeToString(uint64_t microseconds);
std::string pathToString(const std::vector<size_t>& path);
double distance(double x1, double y1, double x2, double y2);
double probability(const Ant& ant, size_t currentPos, size_t futurePos, std::vector<std::vector<Edge>>& edges);
size_t position(const Ant& ant, size_t currentPos, std::vector<std::vector<Edge>>& edges);
size_t findLocallyBestEdge(const Ant& ant, size_t currentPos, std::vector<std::vector<Edge>>& edges);
size_t findBestEdgeBasedOnProbability(const Ant& ant, size_t currentPos, std::vector<std::vector<Edge>>& edges);
bool enoughFiles(uint32_t amount);
void skipLines(std::ifstream& stream, std::size_t lines);
void setParameters(std::ifstream& read);
void getUserInput(uint32_t& experiments, bool& numbersOnly, bool& globalUpdate, bool& localUpdate);