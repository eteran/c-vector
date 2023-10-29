

#define CVECTOR_LOGARITHMIC_GROWTH
#include "cvector.h"
#include "cvector_utils.h"
#include "utest/utest.h"
#include <stdarg.h>
#include <stdlib.h>

UTEST(test, vector_empty) {
    cvector_vector_type(int) v = NULL;
    ASSERT_TRUE(cvector_capacity(v) == 0);
    ASSERT_TRUE(cvector_size(v) == 0);
    ASSERT_TRUE(cvector_begin(v) == NULL);
    ASSERT_TRUE(cvector_end(v) == NULL);
}

UTEST(test, vector_push_pop) {
    cvector_vector_type(int) v = NULL;

    /* add some elements to the back */
    cvector_push_back(v, 10);
    cvector_push_back(v, 20);
    cvector_push_back(v, 30);

    /* and remove one too */
    cvector_pop_back(v);

    ASSERT_TRUE(cvector_capacity(v) == 4);
    ASSERT_TRUE(cvector_size(v) == 2);

    cvector_free(v);
}

UTEST(test, vector_iterator) {
    cvector_vector_type(int) v = NULL;

    /* add some elements to the back */
    cvector_push_back(v, 10);
    cvector_push_back(v, 20);
    cvector_push_back(v, 30);

    /* and remove one too */
    cvector_pop_back(v);

    ASSERT_TRUE(cvector_capacity(v) == 4);
    ASSERT_TRUE(cvector_size(v) == 2);

    /* iterator over the vector using "iterator" style */
    if (v) {
        int *it;
        int i = 0;
        for (it = cvector_begin(v); it != cvector_end(v); ++it) {
            switch (i) {
            case 0:
                ASSERT_TRUE(*it == 10);
                break;
            case 1:
                ASSERT_TRUE(*it == 20);
            }
            ++i;
        }
    }

    cvector_free(v);
}

UTEST(test, vector_index) {
    cvector_vector_type(int) v = NULL;

    /* add some elements to the back */
    cvector_push_back(v, 10);
    cvector_push_back(v, 20);
    cvector_push_back(v, 30);

    /* and remove one too */
    cvector_pop_back(v);

    ASSERT_TRUE(cvector_capacity(v) == 4);
    ASSERT_TRUE(cvector_size(v) == 2);

    /* iterator over the vector standard indexing too! */
    if (v) {
        size_t i;
        for (i = 0; i < cvector_size(v); ++i) {
            switch (i) {
            case 0:
                ASSERT_TRUE(v[i] == 10);
                break;
            case 1:
                ASSERT_TRUE(v[i] == 20);
            }
        }
    }

    cvector_free(v);
}

UTEST(test, vector_insert_delete) {
    cvector_vector_type(int) a = NULL;

    cvector_push_back(a, 1);
    cvector_push_back(a, 5);
    cvector_push_back(a, 4);
    cvector_pop_back(a); // delete 4
    cvector_push_back(a, 5);
    cvector_erase(a, 1); // delete 5
    cvector_erase(a, 0);
    cvector_insert(a, 0, 1);

    ASSERT_TRUE(cvector_capacity(a) == 4);
    ASSERT_TRUE(cvector_size(a) == 2);

    cvector_free(a);
}

UTEST(test, vector_copy) {
    cvector_vector_type(int) a = NULL;
    cvector_vector_type(int) b = NULL;

    cvector_push_back(a, 1);
    cvector_push_back(a, 5);
    cvector_push_back(a, 4);
    cvector_pop_back(a); // delete 4
    cvector_push_back(a, 5);
    cvector_erase(a, 1); // delete 5
    cvector_erase(a, 0);
    cvector_insert(a, 0, 1);

    ASSERT_TRUE(cvector_capacity(a) == 4);
    ASSERT_TRUE(cvector_size(a) == 2);

    if (a) {
        size_t i;
        cvector_copy(a, b);
        ASSERT_TRUE(cvector_size(a) == cvector_size(b));
        for (i = 0; i < cvector_size(b); ++i) {
            ASSERT_TRUE(a[i] == b[i]);
        }
    }

    cvector_free(a);
    cvector_free(b);
}

UTEST(test, vector_reserve) {
    int i;
    cvector_vector_type(int) c = NULL;

    cvector_reserve(c, 100);
    ASSERT_TRUE(cvector_capacity(c) == 100);
    ASSERT_TRUE(cvector_size(c) == 0);

    cvector_push_back(c, 10);
    ASSERT_TRUE(cvector_capacity(c) == 100);
    ASSERT_TRUE(cvector_size(c) == 1);

    cvector_reserve(c, 10);
    ASSERT_TRUE(cvector_capacity(c) == 100);

    for (i = 0; i < 100; ++i) {
        cvector_push_back(c, i);
    }

    ASSERT_TRUE(cvector_capacity(c) == 200);
    cvector_free(c);
}

UTEST(test, vector_free_all) {
    int i;
    cvector_vector_type(char *) v = NULL;
    for (i = 0; i < 10; ++i) {
        char *p = malloc(6);
        strcpy(p, "hello");
        cvector_push_back(v, p);
    }

    ASSERT_TRUE(cvector_size(v) == 10);
    ASSERT_TRUE(cvector_capacity(v) >= 10);

    cvector_free_each_and_free(v, free);
}

UTEST(test, vector_for_each_int) {
    char **it;
    int i;
    cvector_vector_type(char *) v = NULL;
    for (i = 0; i < 10; ++i) {
        char *p = malloc(6);
        strcpy(p, "hello");
        cvector_push_back(v, p);
    }

    ASSERT_TRUE(cvector_size(v) == 10);
    ASSERT_TRUE(cvector_capacity(v) >= 10);

    cvector_for_each_in(it, v) {
        ASSERT_TRUE(strcmp(*it, "hello") == 0);
    }

    cvector_free_each_and_free(v, free);
}

struct data_t {
    int num;
    int a, b, c, d;
};

struct data_t **test(size_t count, ...) {
    cvector_vector_type(struct data_t *) vec = NULL;

    size_t i = 0;

    va_list valist;
    va_start(valist, count);

    for (i = 0; i < count; i++) {
        int num             = va_arg(valist, int);
        struct data_t *data = calloc(sizeof(struct data_t), 1);
        data->num           = num;
        cvector_insert(vec, 0, data);
    }

    va_end(valist);
    return vec;
}

UTEST(test, test_complex_insert) {

    struct data_t **vec = test(4, 1, 2, 3, 4);
    ASSERT_TRUE(cvector_size(vec) == 4);
    ASSERT_TRUE(cvector_capacity(vec) >= 4);
    ASSERT_TRUE(vec[0]->num == 4);
    ASSERT_TRUE(vec[1]->num == 3);
    ASSERT_TRUE(vec[2]->num == 2);
    ASSERT_TRUE(vec[3]->num == 1);
    cvector_free(vec);
}

void cvector_free_destructor(void *p) {
    free(*(void **)p);
}

UTEST(test, derefence_destructor) {
    cvector_vector_type(char *) v = NULL;
    cvector_init(v, 2, cvector_free_destructor, 0);

    char *ptr;
    ptr = strdup("hello");
    ASSERT_TRUE(!!ptr);
    cvector_push_back(v, ptr);

    ptr = strdup("world");
    ASSERT_TRUE(!!ptr);
    cvector_push_back(v, ptr);

    cvector_vector_type(char *) *vec_ptr = &v;
    cvector_free(*vec_ptr);
}

UTEST_MAIN();
