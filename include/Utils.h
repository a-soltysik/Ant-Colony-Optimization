#pragma once
#include "Edge.h"
#include "Point.h"

#include <fstream>
#include <iostream>
#include <random>

class Utils
{
private:
    static std::random_device device;
    static std::mt19937 mt;
    static std::uniform_real_distribution<double> random;

    static std::vector<Point> readPoints(std::ifstream& fin);
    static void setParameters(std::ifstream& fin);
public:
    Utils() = delete;
    static double getRandom0to1() { return random(mt); }
    static uint32_t getRandomUInt(uint32_t from, uint32_t to);
    static bool enoughFiles(uint32_t amount);
    static void skipLines(std::ifstream& fin, std::size_t lines);
    static void getUserInput();

    static std::vector<std::vector<Edge>> parsePointsToEdges(const std::vector<Point>& points) noexcept;
    static std::string timeToString(uint64_t microseconds) noexcept;
    static std::string antsPathToString(const std::vector<size_t>& path) noexcept;
    static double distance(const Point& p1, const Point& p2) noexcept;
    static std::vector<Point> collectData(std::ifstream& fin);
};