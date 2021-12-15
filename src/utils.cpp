#include "utils.h"

// max is inclusive
int randInt(int min, int max)
{
    std::uniform_int_distribution<> distr(min, max);

    return distr(utils_gen);
}
