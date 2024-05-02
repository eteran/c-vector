

#define CVECTOR_LOGARITHMIC_GROWTH
#include "cvector.h"
#include "cvector_utils.h"
#include "utest/utest.h"
#include <stdarg.h>
#include <stdlib.h>

static void free_string(void *ptr) {
    if (ptr) {
        free(*(char **)ptr);
    }
}

static void free_int(void *ptr) {
    if (ptr) {
        free(*(int **)ptr);
    }
}

UTEST(test, cvector_back) {
    cvector_vector_type(int) v = NULL;
    cvector_push_back(v, 0);
    cvector_push_back(v, 1);

    ASSERT_TRUE(cvector_size(v) == 2);

    int *back = cvector_back(v);
    ASSERT_TRUE(*back == 1);

    cvector_push_back(v, 2);

    back = cvector_back(v);
    ASSERT_TRUE(*back == 2);

    cvector_free(v);
}

UTEST(test, cvector_front) {
    cvector_vector_type(int) v = NULL;
    cvector_push_back(v, 0);
    cvector_push_back(v, 1);

    ASSERT_TRUE(cvector_size(v) == 2);

    int *front = cvector_front(v);
    ASSERT_TRUE(*front == 0);

    cvector_erase(v, 0);
    front = cvector_front(v);
    ASSERT_TRUE(*front == 1);

    cvector_free(v);
}

UTEST(test, vector_at) {
    cvector_vector_type(int) v = NULL;
    cvector_push_back(v, 0);
    cvector_push_back(v, 1);
    cvector_push_back(v, 2);
    cvector_push_back(v, 3);
    cvector_push_back(v, 4);

    ASSERT_TRUE(cvector_size(v) == 5);

    if (v) {
        int i = 0;
        for (; i < (int)cvector_size(v); i++) {
            ASSERT_TRUE(*cvector_at(v, i) == i);
        }
    }

    /* test non-exists position */
    int pos_non_exists = 999;
    ASSERT_TRUE(cvector_at(v, pos_non_exists) == NULL);

    /* remove last element*/
    cvector_pop_back(v);
    ASSERT_TRUE(cvector_at(v, 4) == NULL);

    cvector_free(v);
}

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
        cvector_iterator(int) it;
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
    cvector(int) a = NULL;

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

UTEST(test, vector_swap) {
    cvector_vector_type(int) a = NULL;
    cvector_vector_type(int) b = NULL;

    cvector_push_back(a, 1);
    cvector_push_back(a, 2);
    cvector_push_back(a, 3);

    cvector_push_back(b, 4);
    cvector_push_back(b, 5);
    cvector_push_back(b, 6);
    cvector_push_back(b, 7);

    ASSERT_EQ(cvector_size(a), (size_t)3);
    ASSERT_EQ(cvector_size(b), (size_t)4);

    cvector_swap(a, b, int);

    ASSERT_EQ(cvector_size(a), (size_t)4);
    ASSERT_EQ(cvector_size(b), (size_t)3);

    ASSERT_EQ(a[0], 4);
    ASSERT_EQ(a[1], 5);
    ASSERT_EQ(b[0], 1);
    ASSERT_EQ(b[1], 2);
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
    cvector_init(v, 1, free_string);
    for (i = 0; i < 10; ++i) {
        char *p = malloc(6);
        strcpy(p, "hello");
        cvector_push_back(v, p);
    }

    ASSERT_TRUE(cvector_size(v) == 10);
    ASSERT_TRUE(cvector_capacity(v) >= 10);

    cvector_free(v);
}

UTEST(test, vector_for_each_int) {
    cvector_iterator(int *) it;
    int i;
    cvector_vector_type(int *) v = NULL;
    cvector_init(v, 1, free_int);
    for (i = 0; i < 10; ++i) {
        int *p = malloc(sizeof(int));
        *p     = 42;
        cvector_push_back(v, p);
    }

    ASSERT_TRUE(cvector_size(v) == 10);
    ASSERT_TRUE(cvector_capacity(v) >= 10);

    cvector_for_each_in(it, v) {
        /* NOTE(eteran): double pointer because we have an interator to an int*,
         * so first deref to get the int*, the second to get int. Sure, this is a
         * silly thing to do, but this is a test
         */
        ASSERT_TRUE(**it == 42);
    }

    cvector_free(v);
}

UTEST(test, vector_shrink_to_fit) {
    cvector_vector_type(int) a = NULL;

    cvector_push_back(a, 1);
    cvector_push_back(a, 5);
    cvector_push_back(a, 4);

    cvector_reserve(a, 50);
    ASSERT_EQ(cvector_capacity(a), (size_t)50);

    cvector_shrink_to_fit(a);
    ASSERT_EQ(cvector_capacity(a), (size_t)3);

    cvector_free(a);
}

UTEST(test, vector_resize) {
    cvector_vector_type(int) a = NULL;

    cvector_push_back(a, 1);
    cvector_push_back(a, 2);
    cvector_push_back(a, 3);

    cvector_resize(a, 50, 4);
    ASSERT_EQ(cvector_size(a), (size_t)50);
    ASSERT_EQ(a[1], 2);
    ASSERT_EQ(a[30], 4);
    ASSERT_EQ(a[49], 4);
    ASSERT_EQ(a[49], 4);

    cvector_resize(a, 10, 8);
    ASSERT_EQ(cvector_size(a), (size_t)10);
    ASSERT_EQ(a[2], 3);

    cvector_resize(a, 0, 0);
    ASSERT_EQ(cvector_size(a), (size_t)0);

    cvector_free(a);
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
    cvector_init(v, 2, cvector_free_destructor);

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
