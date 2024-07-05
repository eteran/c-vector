[![c-cpp-badge](https://github.com/eteran/c-vector/actions/workflows/cmake.yml/badge.svg)](https://github.com/eteran/c-vector/actions/workflows/cmake.yml)
[![CodeQL](https://github.com/eteran/c-vector/actions/workflows/codeql-analysis.yml/badge.svg?branch=master)](https://github.com/eteran/c-vector/actions/workflows/codeql-analysis.yml)

This is an implementation of a `std::vector` like growable array, but in plain
C89 code. The result is a type safe, easy to use, dynamic array that has a
familiar set of operations.

It works by using the same trick as many allocators, which is to slightly
allocate more data than requested, and using that extra padding in the front
as storage for meta-data. Thus any non-null vector looks like this in memory:

	+-----------------+------+----------+---------+
	| elem_destructor | size | capacity | data... |
	+-----------------+------+----------+---------+
	                                    ^
	                                    | user's pointer

Where the user is given a pointer to first element of `data`. This way the
code has trivial access to the necessary meta-data, but the user need not be
concerned with these details. The total overhead is
`2 * sizeof(size_t) + sizeof(void (*)(void *))` per vector.

To allow the code to be maximally generic, it is implemented as all macros, and
is thus header only. Usage is simple:
```c
/* if this is defined, then the vector will double in capacity each
 * time it runs out of space. if it is not defined, then the vector will
 * be conservative, and will have a capcity no larger than necessary.
 * having this defined will minimize how often realloc gets called.
 */
#define CVECTOR_LOGARITHMIC_GROWTH

#include "cvector.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

	/* this is the variable that will store the array, you can have
	 * a vector of any type! For example, you may write float *v = NULL,
	 * and you'd have a vector of floats :-). NULL will have a size
	 * and capacity of 0. Additionally, vector_begin and vector_end will
	 * return NULL on a NULL vector. Alternatively, for clarity of writing
	 * you can use the cvector_vector_type macro to define a vector of a
	 * given type.
	 */
	cvector_vector_type(int) v = NULL;

	(void)argc;
	(void)argv;

	/* add some elements to the back */
	cvector_push_back(v, 10);
	cvector_push_back(v, 20);
	cvector_push_back(v, 30);
	cvector_push_back(v, 40);

	/* remove an element by specifying an array subscript */
	cvector_erase(v, 2);

	/* remove an element from the back */
	cvector_pop_back(v);

	/* print out some stats about the vector */
	printf("pointer : %p\n", (void *)v);
	printf("capacity: %lu\n", cvector_capacity(v));
	printf("size    : %lu\n", cvector_size(v));

	/* iterator over the vector using "iterator" style */
	if (v) {
		int *it;
		int i = 0;
		for (it = cvector_begin(v); it != cvector_end(v); ++it) {
			printf("v[%d] = %d\n", i, *it);
			++i;
		}
	}

	/* iterator over the vector standard indexing too! */
	if (v) {
		size_t i;
		for (i = 0; i < cvector_size(v); ++i) {
			printf("v[%lu] = %d\n", i, v[i]);
		}
	}

	/* well, we don't have destructors, so let's clean things up */
	cvector_free(v);

	return 0;
}

```

### API

| `std::vector` | `cvector` |
| ------------- | --------- |
| [`std::vector<int> v`](https://en.cppreference.com/w/cpp/container/vector/vector) | `cvector(int) v` |
| [Destructor](https://en.cppreference.com/w/cpp/container/vector/%7Evector) | `cvector_free(v)` |
| [`v.at(3)`](https://en.cppreference.com/w/cpp/container/vector/at) | `cvector_at(v, 3)` |
| [`v[3]`](https://en.cppreference.com/w/cpp/container/vector/operator_at) | `v[3]` |
| [`v.front()`](https://en.cppreference.com/w/cpp/container/vector/front) | `cvector_front(v)` |
| [`v.back()`](https://en.cppreference.com/w/cpp/container/vector/back) | `cvector_back(v)` |
| [`v.begin()`](https://en.cppreference.com/w/cpp/container/vector/begin) | `cvector_begin(v)` |
| [`v.end()`](https://en.cppreference.com/w/cpp/container/vector/begin) | `cvector_end(v)` |
| [`v.empty()`](https://en.cppreference.com/w/cpp/container/vector/empty) | `cvector_empty(v)` |
| [`v.size()`](https://en.cppreference.com/w/cpp/container/vector/size) | `cvector_size(v)` |
| [`v.capacity()`](https://en.cppreference.com/w/cpp/container/vector/capacity) | `cvector_capacity(v)` |
| [`v.shrink_to_fit()`](https://en.cppreference.com/w/cpp/container/vector/shrink_to_fit) | `cvector_shrink_to_fit(v)` |
| [`v.clear()`](https://en.cppreference.com/w/cpp/container/vector/clear) | `cvector_clear(v)` |
| [`v.insert(pos, value)`](https://en.cppreference.com/w/cpp/container/vector/insert) | `cvector_insert(v, pos, value)` |
| [`v.erase(v.begin() + 2)`](https://en.cppreference.com/w/cpp/container/vector/erase) | `cvector_erase(v, 2)` |
| [`v.push_back(value)`](https://en.cppreference.com/w/cpp/container/vector/push_back) | `cvector_push_back(v, value)` |
| [`v.pop_back()`](https://en.cppreference.com/w/cpp/container/vector/pop_back) | `cvector_pop_back(v)` |
| [`v.reserve(new_cap)`](https://en.cppreference.com/w/cpp/container/vector/reserve) | `cvector_reserve(v, new_cap)` |
| [`v.resize(count)`](https://en.cppreference.com/w/cpp/container/vector/resize) | `cvector_resize(v, count)` |
| [`v.swap(other)`](https://en.cppreference.com/w/cpp/container/vector/swap) | `cvector_swap(v, other)` |
| [`std::vector<int> other = v;`](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) | `cvector(int) other; cvector_copy(v, other);` |
