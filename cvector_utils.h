#ifndef CVECTOR_UTILS_H_
#define CVECTOR_UTILS_H_

/**
 * @brief cvector_for_each - call function func on each element of the vector
 * @param vec - the vector
 * @param func - function to be called on each element that takes each element as argument
 * @return void
 */
#define cvector_for_each(vec, func)                          \
    do {                                                     \
        if ((vec) && (func) != NULL) {                       \
            for (size_t i = 0; i < cvector_size(vec); i++) { \
                func((vec)[i]);                              \
            }                                                \
        }                                                    \
    } while (0)

/**
 * @brief cvector_free_each_and_free - calls `free_func` on each element
 * contained in the vector and then destroys the vector itself
 * @param vec - the vector
 * @param free_func - function used to free each element in the vector with
 * one parameter which is the element to be freed)
 * @return void
 */
#define cvector_free_each_and_free(vec, free_func) \
    do {                                           \
        cvector_for_each((vec), (free_func));      \
        cvector_free(vec);                         \
    } while (0)

#endif /* CVECTOR_UTILS_H_ */
