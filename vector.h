
#ifndef VECTOR_H_
#define VECTOR_H_

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#define vector_set_capacity(vec, size)   \
do {                                     \
	if(vec) {                            \
		((size_t *)(vec))[-1] = (size);  \
	}                                    \
} while(0)

#define vector_set_size(vec, size)      \
do {                                    \
	if(vec) {                           \
		((size_t *)(vec))[-2] = (size); \
	}                                   \
} while(0)


#define vector_capacity(vec) \
	((vec) ? ((size_t *)(vec))[-1] : (size_t)0)

#define  vector_size(vec) \
	((vec) ? ((size_t *)(vec))[-2] : (size_t)0)

#define vector_grow(vec, count) \
do {                                                                                              \
	if(!(vec)) {                                                                                  \
		size_t *__p = (size_t *)malloc((count) * sizeof(*(vec)) + (sizeof(size_t) * 2));          \
		assert(__p);                                                                              \
		(vec) = (void *)(&__p[2]);                                                                \
		vector_set_capacity((vec), (count));                                                      \
		vector_set_size((vec), 0);		                                                          \
	} else {                                                                                      \
		size_t *__p1 = &((size_t *)(vec))[-2];                                                    \
		size_t *__p2 = (size_t *)realloc(__p1, ((count) * sizeof(*(vec))+ (sizeof(size_t) * 2))); \
		assert(__p2);                                                                             \
		(vec) = (void *)(&__p2[2]);                                                               \
		vector_set_capacity((vec), (count));                                                      \
	}                                                                                             \
} while(0)

#define vector_pop_back(vec) \
do {                                              \
	vector_set_size((vec), vector_size(vec) - 1); \
} while(0)

#define vector_free(vec) \
do { \
	if(vec) {                                \
		size_t *p1 = &((size_t *)(vec))[-2]; \
		free(p1);                            \
	}                                        \
} while(0)

#define vector_begin(vec) \
	(vec)
	
#define vector_end(vec) \
	&((vec)[vector_size(vec)])


#ifdef LOGARITHMIC_GROWTH

#define vector_push_back(vec, value) \
do {                                                        \
	size_t __cap = vector_capacity(vec);                    \
	if(__cap <= vector_size(vec)) {		                    \
		vector_grow((vec), !__cap ? __cap + 1 : __cap * 2); \
	}                                                       \
	vec[vector_size(vec)] = (value);                        \
	vector_set_size((vec), vector_size(vec) + 1);           \
} while(0)

#else

#define vector_push_back(vec, value) \
do {                                              \
	size_t __cap = vector_capacity(vec);          \
	if(__cap <= vector_size(vec)) {		          \
		vector_grow((vec), __cap + 1);            \
	}                                             \
	vec[vector_size(vec)] = (value);              \
	vector_set_size((vec), vector_size(vec) + 1); \
} while(0)

#endif

#endif
