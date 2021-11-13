#include <math.h>
#include <stdlib.h>

float randomFloat(float lower, float upper) {
	return lower + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(upper-lower)));
}
