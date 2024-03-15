/* if NDEBUG is defined when assert.h is included, the assert() macro
 * does nothing */
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define CVECTOR_LOGARITHMIC_GROWTH

#include "cvector.h"
#include "cvector_utils.h"

static void free_string(void *str) {
    if (str) {
        free(*(char **)str);
    }
}

int main() {

    cvector_vector_type(int) v           = NULL;
    cvector_vector_type(int) a           = NULL;
    cvector_vector_type(int) b           = NULL;
    cvector_vector_type(int) c           = NULL;
    cvector_vector_type(char *) str_vect = NULL;
    cvector_init(str_vect, 1, free_string);

    /* add some elements to the back */
    cvector_push_back(v, 10);
    cvector_push_back(v, 20);
    cvector_push_back(v, 30);

    /* and remove one too */
    cvector_pop_back(v);

    printf("capacity: %zu\n", cvector_capacity(v));
    assert(cvector_capacity(v) == 4);
    printf("size    : %zu\n", cvector_size(v));
    assert(cvector_size(v) == 2);

    /* iterator over the vector using "iterator" style */
    if (v) {
        cvector_iterator(int) it;
        int i = 0;
        for (it = cvector_begin(v); it != cvector_end(v); ++it) {
            printf("v[%d] = %d\n", i, *it);
            switch (i) {
            case 0:
                assert(*it == 10);
                break;
            case 1:
                assert(*it == 20);
            }
            ++i;
        }
    }

    /* iterator over the vector standard indexing too! */
    if (v) {
        size_t i;
        for (i = 0; i < cvector_size(v); ++i) {
            printf("v[%zu] = %d\n", i, v[i]);
            switch (i) {
            case 0:
                assert(v[i] == 10);
                break;
            case 1:
                assert(v[i] == 20);
            }
        }
    }

    /* well, we don't have destructors, so let's clean things up */
    cvector_free(v);

    putchar('\n');

    cvector_push_back(a, 1);
    cvector_push_back(a, 5);
    cvector_push_back(a, 4);
    cvector_pop_back(a); // delete 4
    cvector_push_back(a, 5);
    cvector_erase(a, 1); // delete 5
    cvector_erase(a, 0);
    cvector_insert(a, 0, 1);

    printf("a capacity: %zu\n", cvector_capacity(a));
    assert(cvector_capacity(a) == 4);
    printf("a size    : %zu\n", cvector_size(a));
    assert(cvector_size(a) == 2);

    if (a) {
        size_t i;
        cvector_copy(a, b);
        assert(cvector_size(a) == cvector_size(b));
        for (i = 0; i < cvector_size(b); ++i) {
            printf("a[%zu] = %d\n", i, a[i]);
            assert(a[i] == b[i]);
        }
    }

    cvector_free(a);

    printf("After copy:\n");
    printf("b capacity: %zu\n", cvector_capacity(b));
    assert(cvector_capacity(b) == 2);
    printf("b size    : %zu\n", cvector_size(b));
    assert(cvector_size(b) == 2);
    putchar('\n');

    if (b) {
        size_t i;
        cvector_insert(b, 0, 0);
        cvector_insert(b, 2, 4);
        cvector_insert(b, 2, 2);
        cvector_insert(b, 3, 3);
        printf("b capacity: %zu\n", cvector_capacity(b));
        assert(cvector_capacity(b) == 8);
        printf("b size    : %zu\n", cvector_size(b));
        assert(cvector_size(b) == 6);
        // expected vector: [0, 1, 2, 3, 4, 5]
        for (i = 0; i < cvector_size(b); ++i) {
            printf("b[%zu] = %d\n", i, b[i]);
            assert(b[i] == (int)i);
        }
    }

    cvector_free(b);

    cvector_reserve(c, 100);
    assert(cvector_capacity(c) == 100);
    assert(cvector_size(c) == 0);
    printf("c capacity: %zu\n", cvector_capacity(c));
    printf("c size        : %zu\n", cvector_size(c));
    cvector_push_back(c, 10);
    assert(cvector_capacity(c) == 100);
    assert(cvector_size(c) == 1);
    cvector_reserve(c, 10);
    assert(cvector_capacity(c) == 100);
    printf("c capacity: %zu\n", cvector_capacity(c));
    printf("c size        : %zu\n", cvector_size(c));

    {
        int i;
        for (i = 0; i < 100; ++i) {
            cvector_push_back(c, i);
        }
    }
    assert(cvector_capacity(c) == 200);
    printf("c capacity: %zu\n", cvector_capacity(c));
    printf("c size        : %zu\n", cvector_size(c));
    cvector_free(c);

    cvector_push_back(str_vect, strdup("Hello world"));
    cvector_push_back(str_vect, strdup("Good  bye world"));
    cvector_push_back(str_vect, strdup("not today"));

    if (str_vect) {
        size_t i;
        for (i = 0; i < cvector_size(str_vect); ++i) {
            printf("v[%zu] = %s\n", i, str_vect[i]);
        }
    }

    cvector_free(str_vect);
    return 0;
}
