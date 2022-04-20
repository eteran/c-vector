#ifndef CVECTOR_UTILS_H_
#define CVECTOR_UTILS_H_
/**
 * @brief cvector_call_on_each - call function func on each element of the vector
 * @param vec - the vector
 * @param func - function to be called on each element that takes each element as argument
 * @return void
 */
#define cvector_call_on_each(vec, func)                        \
    do {                                                       \
        if ((vec) && func != NULL) {                           \
            for (size_t i = 0; i < cvector_size((vec)); i++) { \
                func((vec)[i]);                                \
            }                                                  \
        }                                                      \
    } while (0)

/**
 * @brief cvector_free_with_mechanic - frees all memory associated with the vector and call free_func function on each item
 * @param vec - the vector
 * @param free_func - function used to free each element in the vector with one parameter which is the type of the element)
 * @return void
 */
#define cvector_free_and_free_elements(vec, free_func) \
    do {                                               \
        if ((vec) && free_func != NULL) {              \
            cvector_call_on_each(vec, free_func);      \
            cvector_free(vec);                         \
        }                                              \
    } while (0)

#endif /* CVECTOR_UTILS_H_ */
