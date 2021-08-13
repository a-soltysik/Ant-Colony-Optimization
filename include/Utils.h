#pragma once

#include <random>

class Utils
{
private:
    static std::random_device device;
    static std::mt19937 mt;
    static std::uniform_real_distribution<double> random;
public:
    Utils() = delete;
    static double getRandom0to1() { return random(mt); }
};