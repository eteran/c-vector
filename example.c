/* if this is defined, then the vector will double in capacity each
 * time it runs out of space. if it is not defined, then the vector will
 * be conservative, and will have a capcity no larger than necessary.
 * having this defined will minimize how often realloc gets called.
 */
#define LOGARITHMIC_GROWTH

#include "vector.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

  /* this is the variable that will store the array, you can have
   * a vector of any type! For example, you may write float *v = NULL,
   * and you'd have a vector of floats :-). NULL will have a size
   * and capacity of 0. additionally, vector_begin and vector_end will
   * return NULL on a NULL vector.
   */
  int *v = NULL;

  (void)argc;
  (void)argv;

  /* add some elements to the back */
  vector_push_back(v, 10);
  vector_push_back(v, 20);
  vector_push_back(v, 30);

  /* and remove one too */
  vector_pop_back(v);

  /* print out some stats about the vector */
  printf("pointer : %p\n", (void *)v);
  printf("capacity: %lu\n", vector_capacity(v));
  printf("size    : %lu\n", vector_size(v));

  /* iterator over the vector using "iterator" style */
  if (v) {
    int *it;
    int i = 0;
    for (it = vector_begin(v); it != vector_end(v); ++it) {
      printf("v[%d] = %d\n", i, *it);
      ++i;
    }
  }

  /* iterator over the vector standard indexing too! */
  if (v) {
    size_t i;
    for (i = 0; i < vector_size(v); ++i) {
      printf("v[%lu] = %d\n", i, v[i]);
    }
  }

  /* well, we don't have destructors, so let's clean things up */
  vector_free(v);

  const char **sv = NULL;
  vector_set(sv, 1, "one");
  printf("pointer : %p\n", (void *)sv);
  printf("capacity: %lu\n", vector_capacity(sv));
  printf("size    : %lu\n", vector_size(sv));

  vector_set(sv, 3, "three");

  printf("pointer : %p\n", (void *)sv);
  printf("capacity: %lu\n", vector_capacity(sv));
  printf("size    : %lu\n", vector_size(sv));

  if (sv) {
    size_t i;
    for (i = 0; i < vector_size(sv); ++i) {
      printf("v[%lu] = %s\n", i, sv[i]);
    }
  }

  vector_free(sv);

  return 0;
}
