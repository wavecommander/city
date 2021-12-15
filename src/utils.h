#ifndef S_UTILS_H
#define S_UTILS_H

#include <random>

int randInt(int min, int max);

static std::random_device utils_rd;
static std::mt19937 utils_gen(utils_rd());

#endif // S_UTILS_H
