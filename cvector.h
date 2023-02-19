/*
 * Copyright (c) 2015 Evan Teran
 *
 * License: The MIT License (MIT)
 */

#ifndef CVECTOR_H_
#define CVECTOR_H_

#include <assert.h> /* for assert */
#include <stdlib.h> /* for malloc/realloc/free */
#include <string.h> /* for memcpy/memmove */

/* cvector heap implemented using C library malloc() */

/* in case C library malloc() needs extra protection,
 * allow these defines to be overridden.
 */
#ifndef cvector_clib_free
#define cvector_clib_free free
#endif
#ifndef cvector_clib_malloc
#define cvector_clib_malloc malloc
#endif
#ifndef cvector_clib_calloc
#define cvector_clib_calloc calloc
#endif
#ifndef cvector_clib_realloc
#define cvector_clib_realloc realloc
#endif

typedef void (*cvector_elem_destructor_t)(void *elem);
typedef void (*cvector_elem_deep_copy_t)(void *elem_from, void *elem_to);

typedef struct cvector_metadata_t {
    size_t size;
    size_t capacity;
    cvector_elem_destructor_t elem_destructor;
    cvector_elem_deep_copy_t elem_deep_copy;
} cvector_metadata_t;

/**
 * @brief cvector_vector_type - The vector type used in this library
 */
#define cvector_vector_type(type) type *

/**
 * @brief cvector_vec_to_base - For internal use, converts a vector pointer to a metadata pointer
 * @param vec - the vector
 * @return the metadata pointer of the vector
 */
#define cvector_vec_to_base(vec) \
    (&((cvector_metadata_t *)(vec))[-1])

/**
 * @brief cvector_base_to_vec - For internal use, converts a metadata pointer to a vector pointer
 * @param ptr - pointer to the metadata
 * @return the vector
 */
#define cvector_base_to_vec(ptr) \
    ((void *)&((cvector_metadata_t *)(ptr))[1])

/**
 * @brief cvector_capacity - gets the current capacity of the vector
 * @param vec - the vector
 * @return the capacity as a size_t
 */
#define cvector_capacity(vec) \
    ((vec) ? cvector_vec_to_base(vec)->capacity : (size_t)0)

/**
 * @brief cvector_size - gets the current size of the vector
 * @param vec - the vector
 * @return the size as a size_t
 */
#define cvector_size(vec) \
    ((vec) ? cvector_vec_to_base(vec)->size : (size_t)0)

/**
 * @brief cvector_elem_destructor - get the element destructor function used
 * to clean up elements
 * @param vec - the vector
 * @return the function pointer as cvector_elem_destructor_t
 */
#define cvector_elem_destructor(vec) \
    ((vec) ? cvector_vec_to_base(vec)->elem_destructor : NULL)

/**
 * @brief cvector_elem_deep_copy - get the element deep copy function used
 * to make deep copies of elements
 * @param vec - the vector
 * @return the function pointer as cvector_elem_deep_copy_t
 */
#define cvector_elem_deep_copy(vec) \
    ((vec) ? cvector_vec_to_base(vec)->elem_deep_copy : NULL)

/**
 * @brief cvector_empty - returns non-zero if the vector is empty
 * @param vec - the vector
 * @return non-zero if empty, zero if non-empty
 */
#define cvector_empty(vec) \
    (cvector_size(vec) == 0)

/**
 * @brief cvector_reserve - Requests that the vector capacity be at least enough
 * to contain n elements. If n is greater than the current vector capacity, the
 * function causes the container to reallocate its storage increasing its
 * capacity to n (or greater).
 * @param vec - the vector
 * @param capacity - Minimum capacity for the vector.
 * @return void
 */
#define cvector_reserve(vec, capacity)               \
    do {                                             \
        size_t cv_cap__ = cvector_capacity(vec);     \
        if (cv_cap__ < (capacity)) {                 \
            cvector_set_capacity((vec), (capacity)); \
        }                                            \
    } while (0)

/**
 * @brief cvector_erase - removes the element at index i from the vector
 * @param vec - the vector
 * @param i - index of element to remove
 * @return void
 */
#define cvector_erase(vec, i)                                                               \
    do {                                                                                    \
        if (vec) {                                                                          \
            const size_t cv_sz__ = cvector_size(vec);                                       \
            if ((i) < cv_sz__) {                                                            \
                cvector_elem_destructor_t elem_destructor__ = cvector_elem_destructor(vec); \
                if (elem_destructor__) {                                                    \
                    elem_destructor__(&vec[i]);                                             \
                }                                                                           \
                cvector_set_size((vec), cv_sz__ - 1);                                       \
                memmove(                                                                    \
                    (vec) + (i),                                                            \
                    (vec) + (i) + 1,                                                        \
                    sizeof(*(vec)) * (cv_sz__ - 1 - (i)));                                  \
            }                                                                               \
        }                                                                                   \
    } while (0)

/**
 * @brief cvector_erase_fast - Removes the element at index i from the vector by overwriting
 * it with the last element in the vector.  This will change the order of the elements in
 * the vector.
 * @param vec - the vector
 * @param i - index of element to remove
 * @return void
 */
#define cvector_erase_fast(vec, i)                                                          \
    do {                                                                                    \
        if (vec) {                                                                          \
            const size_t cv_sz__ = cvector_size(vec);                                       \
            if ((i) < cv_sz__) {                                                            \
                cvector_elem_destructor_t elem_destructor__ = cvector_elem_destructor(vec); \
                if (elem_destructor__) {                                                    \
                    elem_destructor__(&vec[i]);                                             \
                }                                                                           \
                vec[i] = vec[cv_sz__ - 1];                                                  \
                cvector_set_size((vec), cv_sz__ - 1);                                       \
            }                                                                               \
        }                                                                                   \
    } while (0)

/**
 * @brief cvector_erase_range - Removes count elements from the vector, starting with index i.
 * If the specified range would exceed the size of the vector, elements will only be erased
 * to the end of the vector.
 * @param vec - the vector
 * @param i - first index of elements to remove
 * @param count - the number of elements to remove
 * @return void
 */
#define cvector_erase_range(vec, i, count)                                                  \
    do {                                                                                    \
        if (vec) {                                                                          \
            const size_t cv_sz__ = cvector_size(vec);                                       \
            if ((i) < cv_sz__) {                                                            \
                size_t count__ = (i) + (count) > cv_sz__ ? cv_sz__ - (i) : (count);         \
                cvector_elem_destructor_t elem_destructor__ = cvector_elem_destructor(vec); \
                if (elem_destructor__) {                                                    \
                    size_t i__;                                                             \
                    for (i__ = (i); i__ < (i) + count__; ++i__) {                           \
                        elem_destructor__(&vec[i__]);                                       \
                    }                                                                       \
                }                                                                           \
                cvector_set_size((vec), cv_sz__ - count__);                                 \
                memmove(                                                                    \
                    (vec) + (i),                                                            \
                    (vec) + (i) + count__,                                                  \
                    sizeof(*(vec)) * (cv_sz__ - count__ - (i)));                            \
            }                                                                               \
        }                                                                                   \
    } while (0)

/**
 * @brief cvector_clear - erase all of the elements in the vector
 * @param vec - the vector
 * @return void
 */
#define cvector_clear(vec)                                                              \
    do {                                                                                \
        if (vec) {                                                                      \
            cvector_elem_destructor_t elem_destructor__ = cvector_elem_destructor(vec); \
            if (elem_destructor__) {                                                    \
                size_t i__;                                                             \
                for (i__ = 0; i__ < cvector_size(vec); ++i__) {                         \
                    elem_destructor__(&vec[i__]);                                       \
                }                                                                       \
            }                                                                           \
            cvector_set_size(vec, 0);                                                   \
        }                                                                               \
    } while (0)

/**
 * @brief cvector_free - frees all memory associated with the vector
 * @param vec - the vector
 * @return void
 */
#define cvector_free(vec)                                                               \
    do {                                                                                \
        if (vec) {                                                                      \
            void *p1__                                  = cvector_vec_to_base(vec);     \
            cvector_elem_destructor_t elem_destructor__ = cvector_elem_destructor(vec); \
            if (elem_destructor__) {                                                    \
                size_t i__;                                                             \
                for (i__ = 0; i__ < cvector_size(vec); ++i__) {                         \
                    elem_destructor__(&vec[i__]);                                       \
                }                                                                       \
            }                                                                           \
            cvector_clib_free(p1__);                                                    \
        }                                                                               \
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
 * @brief cvector_compute_next_grow - returns the computed size of the next vector grow
 * size is increased by multiplication of 2
 * @param size - current size
 * @return size after next vector grow
 */
#define cvector_compute_next_grow(size) \
    ((size) ? ((size) << 1) : 1)

#else

/**
 * @brief cvector_compute_next_grow - returns the computed size of the next vector grow
 * size is increased by 1
 * @param size - current size
 * @return size after next vector grow
 */
#define cvector_compute_next_grow(size) \
    ((size) + 1)

#endif /* CVECTOR_LOGARITHMIC_GROWTH */

/**
 * @brief cvector_push_back - adds an element to the end of the vector
 * @param vec - the vector
 * @param value - the value to add
 * @return void
 */
#define cvector_push_back(vec, value)                                         \
    do {                                                                      \
        size_t cv_cap__ = cvector_capacity(vec);                              \
        if (cv_cap__ <= cvector_size(vec)) {                                  \
            cvector_set_capacity((vec), cvector_compute_next_grow(cv_cap__)); \
        }                                                                     \
        (vec)[cvector_size(vec)] = (value);                                   \
        cvector_set_size((vec), cvector_size(vec) + 1);                       \
    } while (0)

/**
 * @brief cvector_insert - insert element at position pos to the vector
 * @param vec - the vector
 * @param pos - position in the vector where the new elements are inserted.
 * @param val - value to be copied (or moved) to the inserted elements.
 * @return void
 */
#define cvector_insert(vec, pos, val)                                         \
    do {                                                                      \
        size_t cv_cap__ = cvector_capacity(vec);                              \
        if (cv_cap__ <= cvector_size(vec)) {                                  \
            cvector_set_capacity((vec), cvector_compute_next_grow(cv_cap__)); \
        }                                                                     \
        if ((pos) < cvector_size(vec)) {                                      \
            memmove(                                                          \
                (vec) + (pos) + 1,                                            \
                (vec) + (pos),                                                \
                sizeof(*(vec)) * ((cvector_size(vec)) - (pos)));              \
        }                                                                     \
        (vec)[(pos)] = (val);                                                 \
        cvector_set_size((vec), cvector_size(vec) + 1);                       \
    } while (0)

/**
 * @brief cvector_pop_back - removes the last element from the vector
 * @param vec - the vector
 * @return void
 */
#define cvector_pop_back(vec)                                                       \
    do {                                                                            \
        cvector_elem_destructor_t elem_destructor__ = cvector_elem_destructor(vec); \
        if (elem_destructor__) {                                                    \
            elem_destructor__(&(vec)[cvector_size(vec) - 1]);                       \
        }                                                                           \
        cvector_set_size((vec), cvector_size(vec) - 1);                             \
    } while (0)

/**
 * @brief cvector_copy - Copy a vector.  If the vector to be copied to has any elements in it,
 * the vector will be cleared using cvector_clear.  If the vector to be copied to has a larger
 * capacity than the vector being copied from, the vector being copied to will not change its
 * capacity.
 * @param from - the original vector
 * @param to - destination to which the function copy to
 * @return void
 */
#define cvector_copy(from, to)                                          \
    do {                                                                \
        if ((from)) {                                                   \
            if ((to)) {                                                 \
                cvector_clear(to);                                      \
            }                                                           \
            if (cvector_capacity(to) < cvector_capacity(from)) {        \
                cvector_set_capacity(to, cvector_size(from));           \
            }                                                           \
            cvector_set_size(to, cvector_size(from));                   \
            memcpy((to), (from), cvector_size(from) * sizeof(*(from))); \
        }                                                               \
    } while (0)

/**
 * @brief cvector_deep_copy - Copy a vector and any deeper memory associated with
 * the vector's elements.  The deep copy function must copy _everything_ in an element,
 * as cvector will not copy anything here.
 * @param from - the original vector
 * @param to - destination vector to copy into
 * @return void
 */
#define cvector_deep_copy(from, to)                                                   \
    do {                                                                              \
        if ((from)) {                                                                 \
            if ((to)) {                                                               \
                cvector_clear(to);                                                    \
            }                                                                         \
            if (cvector_capacity(to) < cvector_capacity(from)) {                      \
                cvector_set_capacity(to, cvector_size(from));                         \
            }                                                                         \
            cvector_set_size(to, cvector_size(from));                                 \
            cvector_elem_deep_copy_t elem_deep_copy__ = cvector_elem_deep_copy(from); \
            if (elem_deep_copy__) {                                                   \
                size_t i__;                                                           \
                for (i__ = 0; i__ < cvector_size(from); ++i__) {                      \
                    elem_deep_copy__(&(from)[i__], &(to)[i__]);                       \
                }                                                                     \
            }                                                                         \
            cvector_set_elem_destructor((to), cvector_elem_destructor(from));         \
            cvector_set_elem_deep_copy((to), cvector_elem_deep_copy(from));           \
        }                                                                             \
    } while (0)

/**
 * @brief cvector_shrink_to_fit - Reduce the vector's capacity to match its current size
 * @param vec - the vector
 * @return void
 */
#define cvector_shrink_to_fit(vec)                    \
    do {                                              \
        cvector_set_capacity(vec, cvector_size(vec)); \
    } while (0)

/**
 * @brief cvector_set_size - For internal use, sets the size variable of the vector
 * @param vec - the vector
 * @param size - the new capacity to set
 * @return void
 */
#define cvector_set_size(vec, _size)                  \
    do {                                              \
        if (vec) {                                    \
            cvector_vec_to_base(vec)->size = (_size); \
        }                                             \
    } while (0)

/**
 * @brief cvector_set_elem_destructor - set the element destructor function
 * used to clean up removed elements
 * @param vec - the vector
 * @param elem_destructor_fn - function pointer of type cvector_elem_destructor_t used to destroy elements
 * @return void
 */
#define cvector_set_elem_destructor(vec, elem_destructor_fn)                  \
    do {                                                                      \
        if (vec) {                                                            \
            cvector_vec_to_base(vec)->elem_destructor = (elem_destructor_fn); \
        }                                                                     \
    } while (0)

/**
 * @brief cvector_set_elem_deep_copy - set the element deep copy function
 * used to make deep copies of elements
 * @param vec - the vector
 * @param elem_deep_copy_fn - function pointer of type cvector_elem_deep_copy_t used to make deep copies of elements
 * @return void
 */
#define cvector_set_elem_deep_copy(vec, elem_deep_copy_fn)                  \
    do {                                                                    \
        if (vec) {                                                          \
            cvector_vec_to_base(vec)->elem_deep_copy = (elem_deep_copy_fn); \
        }                                                                   \
    } while (0)

/**
 * @brief cvector_init - Initialize a vector.  The vector must be NULL for this to do anything.
 * @param vec - the vector
 * @param capacity - vector capacity to reserve
 * @param elem_destructor_fn - element destructor function
 * @param elem_deep_copy_fn - element deep copy function
 * @return void
 */
#define cvector_init(vec, capacity, elem_destructor_fn, elem_deep_copy_fn) \
    do {                                                                   \
        if (!(vec)) {                                                      \
            cvector_set_capacity((vec), capacity);                         \
            cvector_set_elem_destructor((vec), (elem_destructor_fn));      \
            cvector_set_elem_deep_copy((vec), (elem_deep_copy_fn));        \
        }                                                                  \
    } while (0)

/**
 * @brief cvector_set_capacity - Set the vector's capacity to be at least <capacity>
 * elements big. If the new vector capacity is less than the current vector size, then
 * erase the elements at the end of the vector which would be beyond the new capacity.
 * @param vec - the vector
 * @param count - the new capacity to set
 * @return void
 */
#define cvector_set_capacity(vec, count)                                              \
    do {                                                                              \
        const size_t cv_sz__ = (count) * sizeof(*(vec)) + sizeof(cvector_metadata_t); \
        if (vec) {                                                                    \
            if (cvector_size(vec) > (count)) {                                        \
                cvector_erase_range((vec), (count), cvector_size(vec) - (count));     \
            }                                                                         \
            void *cv_p1__ = cvector_vec_to_base(vec);                                 \
            void *cv_p2__ = cvector_clib_realloc(cv_p1__, cv_sz__);                   \
            assert(cv_p2__);                                                          \
            (vec) = cvector_base_to_vec(cv_p2__);                                     \
        } else {                                                                      \
            void *cv_p__ = cvector_clib_malloc(cv_sz__);                              \
            assert(cv_p__);                                                           \
            (vec) = cvector_base_to_vec(cv_p__);                                      \
            cvector_set_size((vec), 0);                                               \
            cvector_set_elem_destructor((vec), NULL);                                 \
            cvector_set_elem_deep_copy((vec), NULL);                                  \
        }                                                                             \
        cvector_vec_to_base(vec)->capacity = (count);                                 \
    } while (0)

#endif /* CVECTOR_H_ */
