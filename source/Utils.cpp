#include "Utils.h"

std::random_device Utils::device;
std::mt19937 Utils::mt(device());
std::uniform_real_distribution<double> Utils::random(0.0, 1.0);

