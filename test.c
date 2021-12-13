/* if NDEBUG is defined when assert.h is included, the assert() macro
 * does nothing */
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdio.h>

#define CVECTOR_LOGARITHMIC_GROWTH

#include "cvector.h"

int main() {
	cvector_vector_type(int) v = NULL;

	/* add some elements to the back */
	cvector_push_back(v, 10);
	cvector_push_back(v, 20);
	cvector_push_back(v, 30);

	/* and remove one too */
	cvector_pop_back(v);

	printf("capacity: %lu\n", cvector_capacity(v));
	assert (cvector_capacity(v) == 4);
	printf("size    : %lu\n", cvector_size(v));
	assert (cvector_size(v) == 2);

	/* iterator over the vector using "iterator" style */
	if (v) {
		int *it;
		int i = 0;
		for (it = cvector_begin(v); it != cvector_end(v); ++it) {
			printf("v[%d] = %d\n", i, *it);
			switch (i)
			{
				case 0:
					assert (*it == 10);
					break;
				case 1:
					assert (*it == 20);
			}
			++i;
		}
	}

	/* iterator over the vector standard indexing too! */
	if (v) {
		size_t i;
		for (i = 0; i < cvector_size(v); ++i) {
			printf("v[%lu] = %d\n", i, v[i]);
			switch (i)
			{
				case 0:
					assert (v[i] == 10);
					break;
				case 1:
					assert (v[i] == 20);
			}
		}
	}

	/* well, we don't have destructors, so let's clean things up */
	cvector_free(v);

	return 0;
}
