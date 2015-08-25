
#define LOGARITHMIC_GROWTH

#include "vector.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

	int *v = NULL;
	
	(void)argc;
	(void)argv;

	vector_push_back(v, 10);
	vector_push_back(v, 20);
	vector_push_back(v, 30);
	vector_pop_back(v);
	vector_push_back(v, 100);
	vector_push_back(v, 200);
	vector_push_back(v, 300);
	vector_push_back(v, 400);
	vector_push_back(v, 500);
	vector_push_back(v, 600);
	vector_push_back(v, 700);		

	printf("pointer : %p\n",  (void *)v);
	printf("capacity: %lu\n", vector_capacity(v));
	printf("size    : %lu\n", vector_size(v));
	
	if(v) {
		int * it;
		int i = 0;
		for(it = vector_begin(v); it != vector_end(v); ++it) {
			printf("v[%d] = %d\n", i, *it);
			++i;
		}
	}
	
	vector_free(v);
	return 0;

}
