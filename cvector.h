
#ifndef CVECTOR_H_
#define CVECTOR_H_

#include <assert.h> /* for assert */
#include <stddef.h> /* for size_t */
#include <stdlib.h> /* for malloc/realloc/free */

/**
 * @brief cvector_vector_type - The vector type used in this library  
 */
#define cvector_vector_type(type) type *

/**
 * @brief cvector_set_capacity - For internal use, sets the capacity variable of the vector
 * @param vec - the vector
 * @param size - the new capacity to set
 * @return void
 */
#define cvector_set_capacity(vec, size)     \
	do {                                    \
		if (vec) {                          \
			((size_t *)(vec))[-1] = (size); \
		}                                   \
	} while (0)

/**
 * @brief cvector_set_size - For internal use, sets the size variable of the vector
 * @param vec - the vector
 * @param size - the new capacity to set
 * @return void
 */
#define cvector_set_size(vec, size)         \
	do {                                    \
		if (vec) {                          \
			((size_t *)(vec))[-2] = (size); \
		}                                   \
	} while (0)

/**
 * @brief cvector_capacity - gets the current capacity of the vector
 * @param vec - the vector
 * @return the capacity as a size_t
 */
#define cvector_capacity(vec) \
	((vec) ? ((size_t *)(vec))[-1] : (size_t)0)

/**
 * @brief cvector_size - gets the current size of the vector
 * @param vec - the vector
 * @return the size as a size_t
 */
#define cvector_size(vec) \
	((vec) ? ((size_t *)(vec))[-2] : (size_t)0)

/**
 * @brief cvector_empty - returns non-zero if the vector is empty
 * @param vec - the vector
 * @return non-zero if empty, zero if non-empty
 */
#define cvector_empty(vec) \
	(cvector_size(vec) == 0)

/**
 * @brief cvector_grow - For internal use, ensures that the vector is at least <count> elements big
 * @param vec - the vector
 * @param size - the new capacity to set
 * @return void
 */
#define cvector_grow(vec, count)                                             \
	do {                                                                     \
		const size_t __sz = (count) * sizeof(*(vec)) + (sizeof(size_t) * 2); \
		if (!(vec)) {                                                        \
			size_t *__p = malloc(__sz);                                      \
			assert(__p);                                                     \
			(vec) = (void *)(&__p[2]);                                       \
			cvector_set_capacity((vec), (count));                            \
			cvector_set_size((vec), 0);                                      \
		} else {                                                             \
			size_t *__p1 = &((size_t *)(vec))[-2];                           \
			size_t *__p2 = realloc(__p1, (__sz));                            \
			assert(__p2);                                                    \
			(vec) = (void *)(&__p2[2]);                                      \
			cvector_set_capacity((vec), (count));                            \
		}                                                                    \
	} while (0)

/**
 * @brief cvector_pop_back - removes the last element from the vector
 * @param vec - the vector
 * @return void
 */
#define cvector_pop_back(vec)                           \
	do {                                                \
		cvector_set_size((vec), cvector_size(vec) - 1); \
	} while (0)

/**
 * @brief cvector_erase - removes the element at index i from the vector
 * @param vec - the vector
 * @param i - index of element to remove
 * @return void
 */
#define cvector_erase(vec, i)                              \
	do {                                                   \
		if (vec) {                                         \
			const size_t __sz = cvector_size(vec);         \
			if ((i) < __sz) {                              \
				cvector_set_size((vec), __sz - 1);         \
				size_t __x;                                \
				for (__x = (i); __x < (__sz - 1); ++__x) { \
					(vec)[__x] = (vec)[__x + 1];           \
				}                                          \
			}                                              \
		}                                                  \
	} while (0)

/**
 * @brief cvector_free - frees all memory associated with the vector
 * @param vec - the vector
 * @return void
 */
#define cvector_free(vec)                        \
	do {                                         \
		if (vec) {                               \
			size_t *p1 = &((size_t *)(vec))[-2]; \
			free(p1);                            \
		}                                        \
	} while (0)

/**
 * @brief cvector_begin - returns an iterator to first element of the vector
 * @param vec - the vector
 * @return a pointer to the first element (or NULL)
 */
#define cvector_begin(vec) \
	(vec)

/**
 * @brief cvector_end - returns an iterator to one past the last element of the vector
 * @param vec - the vector
 * @return a pointer to one past the last element (or NULL)
 */
#define cvector_end(vec) \
	((vec) ? &((vec)[cvector_size(vec)]) : NULL)

/* user request to use logarithmic growth algorithm */
#ifdef CVECTOR_LOGARITHMIC_GROWTH

/**
 * @brief cvector_push_back - adds an element to the end of the vector
 * @param vec - the vector
 * @param value - the value to add 
 * @return void
 */
#define cvector_push_back(vec, value)                            \
	do {                                                         \
		size_t __cap = cvector_capacity(vec);                    \
		if (__cap <= cvector_size(vec)) {                        \
			cvector_grow((vec), !__cap ? __cap + 1 : __cap * 2); \
		}                                                        \
		vec[cvector_size(vec)] = (value);                        \
		cvector_set_size((vec), cvector_size(vec) + 1);          \
	} while (0)

#else

/**
 * @brief cvector_push_back - adds an element to the end of the vector
 * @param vec - the vector
 * @param value - the value to add 
 * @return void
 */
#define cvector_push_back(vec, value)                   \
	do {                                                \
		size_t __cap = cvector_capacity(vec);           \
		if (__cap <= cvector_size(vec)) {               \
			cvector_grow((vec), __cap + 1);             \
		}                                               \
		vec[cvector_size(vec)] = (value);               \
		cvector_set_size((vec), cvector_size(vec) + 1); \
	} while (0)

#endif /* CVECTOR_LOGARITHMIC_GROWTH */

#endif /* CVECTOR_H_ */
