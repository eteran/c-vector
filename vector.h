
#ifndef VECTOR_H_
#define VECTOR_H_

#include <assert.h> /* for assert */
#include <stddef.h> /* for size_t */
#include <stdlib.h> /* for malloc/realloc/free */
#include <string.h> /* for memset */

/**
 * @brief vector_set_capacity - For internal use, sets the capacity variable of
 * the vector
 * @param vec - the vector
 * @param size - the new capacity to set
 * @return void
 */
#define vector_set_capacity(vec, size)                                         \
  do {                                                                         \
    if (vec) {                                                                 \
      ((size_t *)(vec))[-1] = (size);                                          \
    }                                                                          \
  } while (0)

/**
 * @brief vector_set_size - For internal use, sets the size variable of the
 * vector
 * @param vec - the vector
 * @param size - the new size to set
 * @return void
 */
#define vector_set_size(vec, size)                                             \
  do {                                                                         \
    if (vec) {                                                                 \
      ((size_t *)(vec))[-2] = (size);                                          \
    }                                                                          \
  } while (0)

/**
 * @brief vector_capacity - gets the current capacity of the vector
 * @param vec - the vector
 * @return the capacity as a size_t
 */
#define vector_capacity(vec) ((vec) ? ((size_t *)(vec))[-1] : (size_t)0)

/**
 * @brief vector_size - gets the current size of the vector
 * @param vec - the vector
 * @return the size as a size_t
 */
#define vector_size(vec) ((vec) ? ((size_t *)(vec))[-2] : (size_t)0)

/**
 * @brief vector_empty - returns non-zero if the vector is empty
 * @param vec - the vector
 * @return non-zero if empty, zero if non-empty
 */
#define vector_empty(vec) (vector_size(vec) == 0)

/**
 * @brief vector_grow - For internal use, ensures that the vector is at least
 * <count> elements big
 * @param vec - the vector
 * @param size - the new capacity to set
 * @return void
 */
#define vector_grow(vec, count)                                                \
  do {                                                                         \
    const size_t __size = (count) * sizeof(*(vec)) + (sizeof(size_t) * 2);     \
    if (!(vec)) {                                                              \
      size_t *__p = malloc(__size);                                            \
      assert(__p);                                                             \
      memset(__p, 0, __size);                                                  \
      (vec) = (void *)(&__p[2]);                                               \
      vector_set_capacity((vec), (count));                                     \
      vector_set_size((vec), 0);                                               \
    } else {                                                                   \
      const size_t __prev_size =                                               \
          vector_capacity(vec) * sizeof(*(vec)) + sizeof(size_t) * 2;          \
      size_t *__p1 = &((size_t *)(vec))[-2];                                   \
      size_t *__p2 = realloc(__p1, __size);                                    \
      assert(__p2);                                                            \
      memset((char *)__p2 + __prev_size, 0, __size - __prev_size);             \
      (vec) = (void *)(&__p2[2]);                                              \
      vector_set_capacity((vec), (count));                                     \
    }                                                                          \
  } while (0)

/**
 * @brief vector_pop_back - removes the last element from the vector
 * @param vec - the vector
 * @return void
 */
#define vector_pop_back(vec)                                                   \
  do {                                                                         \
    vector_set_size((vec), vector_size(vec) - 1);                              \
  } while (0)

/**
 * @brief vector_erase - removes the element at index i from the vector
 * @param vec - the vector
 * @param i - index of element to remove
 * @return void
 */
#define vector_erase(vec, i)                                                   \
  do {                                                                         \
    if (vec) {                                                                 \
      const size_t __sz = vector_size(vec);                                    \
      if ((i) < __sz) {                                                        \
        vector_set_size((vec), __sz - 1);                                      \
        size_t __x;                                                            \
        for (__x = (i); __x < (__sz - 1); ++__x) {                             \
          (vec)[__x] = (vec)[__x + 1];                                         \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  } while (0)

/**
 * @brief vector_free - frees all memory associated with the vector
 * @param vec - the vector
 * @return void
 */
#define vector_free(vec)                                                       \
  do {                                                                         \
    if (vec) {                                                                 \
      size_t *p1 = &((size_t *)(vec))[-2];                                     \
      free(p1);                                                                \
    }                                                                          \
  } while (0)

/**
 * @brief vector_begin - returns an iterator to first element of the vector
 * @param vec - the vector
 * @return a pointer to the first element (or NULL)
 */
#define vector_begin(vec) (vec)

/**
 * @brief vector_end - returns an iterator to one past the last element of the
 * vector
 * @param vec - the vector
 * @return a pointer to one past the last element (or NULL)
 */
#define vector_end(vec) ((vec) ? &((vec)[vector_size(vec)]) : NULL)

#ifdef LOGARITHMIC_GROWTH

#define vector_maybe_grow(vec, index)                                          \
  do {                                                                         \
    size_t __cap = vector_capacity(vec);                                       \
    if (__cap <= index) {                                                      \
      if (!__cap)                                                              \
        __cap = 1;                                                             \
      while (__cap <= index)                                                   \
        __cap = __cap << 1;                                                    \
      vector_grow((vec), __cap);                                               \
    }                                                                          \
  } while (0)

#else

#define vector_maybe_grow(vec, capacity)                                       \
  do {                                                                         \
    const size_t __cap = vector_capacity(vec);                                 \
    if (__cap <= vector_size(vec)) {                                           \
      vector_grow((vec), __cap + 1);                                           \
    }                                                                          \
  } while (0)

#endif

/**
 * @brief vector_push_back - adds an element to the end of the vector
 * @param vec - the vector
 * @param value - the value to add
 * @return void
 */
#define vector_push_back(vec, value)                                           \
  do {                                                                         \
    const size_t __new_size = vector_size(vec) + 1;                            \
    vector_maybe_grow(vec, __new_size);                                        \
    vec[vector_size(vec)] = (value);                                           \
    vector_set_size((vec), vector_size(vec) + 1);                              \
  } while (0)

#define vector_set(vec, i, value)                                              \
  vector_maybe_grow(vec, i);                                                   \
  vec[i] = (value);                                                            \
  vector_set_size((vec), i + 1);

#endif
