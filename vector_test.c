#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "vector.h"

#define STDERR(...) fprintf(stderr, __VA_ARGS__)
#define STDOUT(...) fprintf(stdout, __VA_ARGS__)

VECTOR_DECLARE(static, vector_int, int)
VECTOR_DEFINE(static, vector_int, int)

static int
int_compare(int x, int y)
{
	if (x != y)
		return x < y ? -1 : 1;

	return 0;
}

static int
test_quicksort(int size, int n_tests)
{
	vector_int_t v;

	STDOUT("Running quicksort test...\n");

	for (int test_n = 1; test_n <= n_tests; test_n++) {

		// STDOUT("Running test %d\n", test_n);

		vector_int_init(&v);

		for (int i = 0; i < size; i++) {
			int err = vector_int_push(&v, rand());
			if (err) {
				int e = errno;
				STDERR("vector_int_push failed\n");
				STDERR("%s\n", strerror(e));
				STDOUT("Quicksort failed\n");
				vector_int_destroy(&v);
				return -1;
			}
		}

		vector_int_quicksort(&v, int_compare);

		for (int i = 1; i < size; i++) {
			if (v.arr[i - 1] > v.arr[i]) {
				STDOUT("Quicksort failed\n");
				vector_int_destroy(&v);
				return -1;
			}
		}

		vector_int_destroy(&v);
	}

	STDOUT("Quicksort passed\n");
	return 0;
}

static int
test_push_pop(int size, int n_tests)
{
	vector_int_t v;

	STDOUT("Running push/pop test...\n");

	for (int test_n = 1; test_n <= n_tests; test_n++) {

		// STDOUT("Running test %d\n", test_n);

		vector_int_init(&v);

		for (int i = 0; i < size; i++) {
			int err = vector_int_push(&v, i);
			if (err) {
				int e = errno;
				STDERR("vector_int_push failed\n");
				STDERR("%s\n", strerror(e));
				STDOUT("Push/Pop failed\n");
				vector_int_destroy(&v);
				return -1;
			}
		}

		for (int i = size - 1; i >= 0; i--) {
			int x;
			int err = vector_int_pop(&v, &x);

			if (err) {
				STDERR("vector_int_pop: %s\n", strerror(errno));
				STDOUT("Push/Pop failed\n");
				vector_int_destroy(&v);
				return -1;
			}

			if (x != i) {
				STDOUT("Push/Pop failed\n");
				vector_int_destroy(&v);
				return -1;
			}
		}

		vector_int_destroy(&v);
	}

	STDOUT("Push/Pop passed\n");
	return 0;
}

static int
test_insert_remove_fast(int size, int n_tests)
{
	vector_int_t *v;

	STDOUT("Running insert/remove fast test...\n");

	for (int test_n = 1; test_n <= n_tests; test_n++) {

		v = vector_int_alloc();

		for (int i = 0; i < size; i++) {
			int err = vector_int_insert_fast(v, 0, i);
			if (err) {
				STDERR("vector_int_insert_fast: %s\n", strerror(errno));
				STDOUT("Insert/Remove Fast failed\n");
				vector_int_free(v);
				return -1;
			}
		}

		for (int i = size - 1; i >= 0; i--) {
			int x;
			int err = vector_int_remove_fast(v, 0, &x);
			if (err) {
				STDERR("vector_int_remove_fast: %s\n", strerror(errno));
				STDOUT("Insert/Remove Fast failed\n");
				vector_int_free(v);
				return -1;
			}
		}

		vector_int_free(v);
	}

	STDOUT("Insert/Remove Fast passed\n");
	return 0;
}

static int
test_insert_remove(int size, int n_tests)
{
	vector_int_t v;

	STDOUT("Running insert/remove test...\n");

	for (int test_n = 1; test_n <= n_tests; test_n++) {

		vector_int_init(&v);

		for (int i = 0; i < size; i++) {
			int err = vector_int_insert(&v, 0, i);
			if (err) {
				STDERR("vector_int_insert: %s\n", strerror(errno));
				STDOUT("Insert/Remove failed\n");
				vector_int_destroy(&v);
				return -1;
			}
		}

		for (int i = size - 1; i >= 0; i--) {
			int x;
			int err = vector_int_remove(&v, 0, &x);
			if (err) {
				STDERR("vector_int_remove: %s\n", strerror(errno));
				STDOUT("Insert/Remove failed\n");
				vector_int_destroy(&v);
				return -1;
			}
		}

		vector_int_destroy(&v);
	}

	STDOUT("Insert/Remove passed\n");
	return 0;
}

static int
test_index(int size, int n_tests)
{
	vector_int_t v;
	int err;

	STDOUT("Running index/set_index test...\n");

	for (int test_n = 1; test_n <= n_tests; test_n++) {

		vector_int_init(&v);

		err = vector_int_set_len(&v, size);
		if (err) {
			STDERR("vector_int_set_len: %s\n", strerror(errno));
			vector_int_destroy(&v);
			return -1;
		}

		for (int i = 0; i < size; i++) {
			err = vector_int_set_index(&v, i, i);
			if (err) {
				STDERR("vector_int_set_index: %s\n", strerror(errno));
				STDOUT("Index/Set_index failed\n");
				vector_int_destroy(&v);
				return -1;
			}
		}

		for (int i = size - 1; i >= 0; i--) {
			int x;
			err = vector_int_index(&v, i, &x);
			if (err) {
				STDERR("vector_int_index: %s\n", strerror(errno));
				STDOUT("Index/Set_index failed\n");
				vector_int_destroy(&v);
				return -1;
			}

			if (x != i) {
				STDOUT("Index/Set_index failed\n");
				vector_int_destroy(&v);
				return -1;
			}
		}

		vector_int_destroy(&v);
	}

	STDOUT("Index/Set_index passed\n");
	return 0;
}

int
main(void)
{
	test_push_pop(10000, 1000);
	test_insert_remove(10000, 1000);
	test_insert_remove_fast(10000, 1000);
	test_index(10000, 1000);
	test_quicksort(10000, 1000);
	exit(0);
}
