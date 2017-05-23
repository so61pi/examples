#include "libexample.h"


double avg(double const* array, unsigned size) {
	double sum = 0;
	for (unsigned i = 0; i < size; ++i) {
		sum += array[i];
	}
	return sum / size;
}
