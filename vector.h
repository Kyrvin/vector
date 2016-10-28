/* Copyright (c) 2016, Patrick Keating <kyrvin3@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <errno.h>
#include <stdlib.h>
#include <string.h>

/* For the documentation on VECTOR_DECLARE and VECTOR_DEFINE,
 * jump to the bottom of this file.
 */

#define _VECTOR_MAX(x, y) ((x) > (y) ? (x) : (y))

/* Type
 *
 * Defines the vector struct containing the fields len, cap, and arr. The
 * struct is typedefed to namespace_t.
 */
#define _VECTOR_DEFINE_TYPE(vect_t, base_t) \
	typedef struct vect_t {size_t len, cap; base_t *arr; } vect_t;

/* Init
 *
 * Initialize a statically allocated vector, returns the vector. This can also
 * be acheived using the VECTOR_INITIALIZER macro above.
 */
#define _VECTOR_DECLARE_INIT(namespace, base_t, vect_t) \
	vect_t * namespace ## _init (vect_t *v)

#define _VECTOR_DEFINE_INIT(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_INIT(namespace, base_t, vect_t) \
	{ \
		v->len = 0; \
		v->cap = 0; \
		v->arr = NULL; \
		return v; \
	}

/* Alloc
 *
 * Allocates a new vector on the heap, returning a pointer to that vector.
 */
#define _VECTOR_DECLARE_ALLOC(namespace, base_t, vect_t) \
	vect_t * namespace ## _alloc (void)

#define _VECTOR_DEFINE_ALLOC(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_ALLOC(namespace, base_t, vect_t) \
	{ \
		vect_t *v = malloc(sizeof(vect_t)); \
		if (!v) \
			return NULL; \
	\
		namespace ## _init (v); \
		return v; \
	}

/* Destroy
 *
 * Destroy a statically allocated vector.
 */
#define _VECTOR_DECLARE_DESTROY(namespace, base_t, vect_t) \
	void namespace ## _destroy (vect_t *v)

#define _VECTOR_DEFINE_DESTROY(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_DESTROY(namespace, base_t, vect_t) \
	{ \
		free(v->arr); \
	}

/* Free
 *
 * Free a vector allocated on the heap.
 */
#define _VECTOR_DECLARE_FREE(namespace, base_t, vect_t) \
	void namespace ## _free (vect_t *v)

#define _VECTOR_DEFINE_FREE(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_FREE(namespace, base_t, vect_t) \
	{ \
		if (!v) \
			return; \
	\
		namespace ## _destroy (v); \
		free(v); \
	}

/* Expand
 *
 * Expand the vector to 'new_cap', which will be rounded up to the next power
 * of 2. Returns 0 on success or -1 on an allocation failure.
 */
#define _VECTOR_DECLARE_EXPAND(namespace, base_t, vect_t) \
	int namespace ## _expand (vect_t *v, size_t new_cap)

#define _VECTOR_DEFINE_EXPAND(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_EXPAND(namespace, base_t, vect_t) \
	{ \
		base_t *tmp; \
		/* Round new_cap up to the next power of 2 */ \
		/* From https://graphics.stanford.edu/~seander/bithacks.html */ \
		new_cap--; \
		new_cap |= new_cap >> 1; \
		new_cap |= new_cap >> 2; \
		new_cap |= new_cap >> 4; \
		new_cap |= new_cap >> 8; \
		new_cap |= new_cap >> 16; \
		new_cap++; \
	\
		if (new_cap <= v->cap) \
			return 0; \
	\
		tmp = realloc(v->arr, new_cap * sizeof(base_t)); \
		if (!tmp) \
			return -1; \
	\
		v->arr = tmp; \
		v->cap = new_cap; \
		return 0; \
	}

/* Set Len
 *
 * Set the length of the vector to 'len'. If 'len' is greater than the current
 * capacity of the vector, the vector is automatically reallocated using
 * _expand(). If 'len' is greater than the current length, the new
 * elements are unitialized. Returns 0 on success, or -1 if on an allocation
 * failure.
 */
#define _VECTOR_DECLARE_SET_LEN(namespace, base_t, vect_t) \
	int namespace ## _set_len (vect_t *v, size_t len)

#define _VECTOR_DEFINE_SET_LEN(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_SET_LEN(namespace, base_t, vect_t) \
	{ \
		if (len > v->cap && namespace ## _expand (v, len)) \
			return -1; \
	\
		v->len = len; \
		return 0; \
	}

/* Len
 *
 * Returns the length of the vector.
 */
#define _VECTOR_DECLARE_LEN(namespace, base_t, vect_t) \
	size_t namespace ## _len (vect_t *v)

#define _VECTOR_DEFINE_LEN(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_LEN(namespace, base_t, vect_t) \
	{ \
		return v->len; \
	}

/* Cap
 *
 * Returns the capacity of the vector.
 */
#define _VECTOR_DECLARE_CAP(namespace, base_t, vect_t) \
	size_t namespace ## _cap (vect_t *v)

#define _VECTOR_DEFINE_CAP(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_CAP(namespace, base_t, vect_t) \
	{ \
		return v->cap; \
	}

/* Push
 *
 * Pushes 'x' to the end of the vector, automatically reallocating the vector
 * if necessary. Returns 0 on success or -1 on an allocation failure.
 */
#define _VECTOR_DECLARE_PUSH(namespace, base_t, vect_t) \
	int namespace ## _push (vect_t *v, base_t x)

#define _VECTOR_DEFINE_PUSH(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_PUSH(namespace, base_t, vect_t) \
	{ \
		if (v->len == v->cap && namespace ## _expand (v, v->len + 1)) \
			return -1; \
	\
		v->arr[v->len++] = x; \
		return 0; \
	}

/* Pop
 *
 * Pop an element off the end of the vector, storing the value in 'out'. Out
 * may be NULL. If the length of the vector is 0, -1 is returned and 'out' is
 * not changed. Returns 0 on success.
 */
#define _VECTOR_DECLARE_POP(namespace, base_t, vect_t) \
	int namespace ## _pop (vect_t *v, base_t *out)

#define _VECTOR_DEFINE_POP(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_POP(namespace, base_t, vect_t) \
	{ \
		if (!v->len) { \
			errno = ERANGE; \
			return -1; \
		} \
	\
		if (out) \
			*out = v->arr[v->len - 1]; \
	\
		v->len--; \
		return 0; \
	}

/* Swap
 *
 * Swap indices 'i' and 'j'. If either 'i' or 'j' is outside the vector, the
 * vector is untouched and -1 is returned.
 */
#define _VECTOR_DECLARE_SWAP(namespace, base_t, vect_t) \
	int namespace ## _swap (vect_t *v, size_t i, size_t j)

#define _VECTOR_DEFINE_SWAP(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_SWAP(namespace, base_t, vect_t) \
	{ \
		base_t tmp; \
	\
		if (_VECTOR_MAX(i, j) >= v->len) { \
			errno = ERANGE; \
			return -1; \
		} \
	\
		tmp = v->arr[i]; \
		v->arr[i] = v->arr[j]; \
		v->arr[j] = tmp; \
		return 0; \
	}

/* Index
 *
 * Get index 'i' from the vector and store it in 'out'. If 'i' is outside the
 * range of the vector, -1 is returned and 'out' is not set.
 */
#define _VECTOR_DECLARE_INDEX(namespace, base_t, vect_t) \
	int namespace ## _index (vect_t *v, size_t i, base_t *out)

#define _VECTOR_DEFINE_INDEX(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_INDEX(namespace, base_t, vect_t) \
	{ \
		if (i >= v->len) { \
			errno = ERANGE; \
			return -1; \
		} \
	\
		if (out) \
			*out = v->arr[i]; \
	\
		return 0; \
	}

/* Set Index
 *
 * Set index 'i' to 'x'. If 'i' is outside the length of the vector, -1 is
 * returned and 'x' is not placed in the vector.
 */
#define _VECTOR_DECLARE_SET_INDEX(namespace, base_t, vect_t) \
	int namespace ## _set_index (vect_t *v, size_t i, base_t x)

#define _VECTOR_DEFINE_SET_INDEX(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_SET_INDEX(namespace, base_t, vect_t) \
	{ \
		if (i >= v->len) { \
			errno = ERANGE; \
			return -1; \
		} \
	\
		v->arr[i] = x; \
		return 0; \
	}

/* Insert Fast
 *
 * Insert 'x' into index 'i', appending the old value in 'i' to the end of the
 * vector. If 'i' equals the length of the vector, this function does not fail,
 * and behaves exactly the same as _push(). Returns -1 on failure with
 * errno set to ENOMEM on a memory error or ERANGE if 'i' is outside the
 * range of the vector.
 */
#define _VECTOR_DECLARE_INSERT_FAST(namespace, base_t, vect_t) \
	int namespace ## _insert_fast (vect_t *v, size_t i, base_t x)

#define _VECTOR_DEFINE_INSERT_FAST(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_INSERT_FAST(namespace, base_t, vect_t) \
	{ \
		if (i > v->len) { \
			errno = ERANGE; \
			return -1; \
		} \
	\
		if (namespace ## _set_len (v, _VECTOR_MAX(v->len, i) + 1)) \
			return -1; \
	\
		v->arr[v->len - 1] = v->arr[i]; \
		v->arr[i] = x; \
		return 0; \
	}

/* Insert
 *
 * Behaves the same as _insert_fast(), but shifts all elements up
 * one position to preserve order.
 */
#define _VECTOR_DECLARE_INSERT(namespace, base_t, vect_t) \
	int namespace ## _insert (vect_t *v, size_t i, base_t x)

#define _VECTOR_DEFINE_INSERT(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_INSERT(namespace, base_t, vect_t) \
	{ \
		if (i > v->len) { \
			errno = ERANGE; \
			return -1; \
		} \
	\
		if (namespace ## _set_len (v, _VECTOR_MAX(v->len, i) + 1)) \
			return -1; \
	\
		memmove(&v->arr[i + 1], &v->arr[i], (v->len - 1 - i) * sizeof(base_t)); \
		v->arr[i] = x; \
		return 0; \
	}

/* Remove Fast
 *
 * Removes the element in index 'i' from the vector and stores it in 'out'.
 * The last element in the vector is inserted into index 'i'. If 'i' is not
 * a valid index of the vector, -1 is returned.
 */
#define _VECTOR_DECLARE_REMOVE_FAST(namespace, base_t, vect_t) \
	int namespace ## _remove_fast (vect_t *v, size_t i, base_t *out)

#define _VECTOR_DEFINE_REMOVE_FAST(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_REMOVE_FAST(namespace, base_t, vect_t) \
	{ \
		if (i >= v->len) { \
			errno = ERANGE; \
			return -1; \
		} \
	\
		if (out) \
			*out = v->arr[i]; \
	\
		v->arr[i] = v->arr[v->len - 1]; \
		v->len--; \
		return 0; \
	}

/* Remove
 *
 * Behaves the same as _remove_fast(), but shifts all elements in the
 * vector down one position to preserve order.
 */
#define _VECTOR_DECLARE_REMOVE(namespace, base_t, vect_t) \
	int namespace ## _remove (vect_t *v, size_t i, base_t *out)

#define _VECTOR_DEFINE_REMOVE(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_REMOVE(namespace, base_t, vect_t) \
	{ \
		if (i >= v->len) { \
			errno = ERANGE; \
			return -1; \
		} \
	\
		if (out) \
			*out = v->arr[i]; \
	\
		memmove(&v->arr[i], &v->arr[i + 1], (v->len - i) * sizeof(base_t)); \
		v->len--; \
		return 0; \
	}

/* Quicksort
 *
 * Takes a vector and a comparison function and sorts the list using
 * quicksort. The comparison function should return -1 if the first element is
 * less than the second argument, 1 if the first element is larger, and 0 if
 * they are equal.
 */
#define _VECTOR_DECLARE_QUICKSORT(namespace, base_t, vect_t) \
	void namespace ## _quicksort (vect_t *v, int (*compare) (base_t, base_t))

#define _VECTOR_DEFINE_QUICKSORT(namespace, base_t, vect_t) \
	_VECTOR_DECLARE_QUICKSORT(namespace, base_t, vect_t) \
	{ \
		base_t piv; \
		size_t lo, hi; \
		vect_t v1, v2; \
	\
		if (v->len < 2) \
			return; \
	\
		piv = v->arr[v->len / 2]; \
		for (lo = 0, hi = v->len - 1;; lo++, hi--) { \
			while (compare(v->arr[lo], piv) < 0) lo++; \
			while (compare(v->arr[hi], piv) > 0) hi--; \
			if (lo >= hi) \
				break; \
			namespace ## _swap (v, lo, hi); \
		} \
		v1.len = lo; \
		v1.arr = v->arr; \
		v2.len = v->len - v1.len; \
		v2.arr = v->arr + v1.len; \
	\
		namespace ## _quicksort(&v1, compare); \
		namespace ## _quicksort(&v2, compare); \
		return; \
	}

/*
 * Do Declare
 */
#define _VECTOR_DO_DECLARE(how, namespace, base_t, vect_t) \
	_VECTOR_DEFINE_TYPE(vect_t, base_t) \
	how _VECTOR_DECLARE_INIT(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_ALLOC(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_DESTROY(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_FREE(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_EXPAND(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_SET_LEN(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_LEN(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_CAP(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_PUSH(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_POP(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_SWAP(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_INDEX(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_SET_INDEX(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_INSERT_FAST(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_INSERT(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_REMOVE_FAST(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_REMOVE(namespace, base_t, vect_t); \
	how _VECTOR_DECLARE_QUICKSORT(namespace, base_t, vect_t);

/*
 * Do Define
 */
#define _VECTOR_DO_DEFINE(how, namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_INIT(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_ALLOC(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_DESTROY(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_FREE(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_EXPAND(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_SET_LEN(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_LEN(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_CAP(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_PUSH(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_POP(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_SWAP(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_INDEX(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_SET_INDEX(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_INSERT_FAST(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_INSERT(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_REMOVE_FAST(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_REMOVE(namespace, base_t, vect_t) \
	how _VECTOR_DEFINE_QUICKSORT(namespace, base_t, vect_t)

/* Declare
 *
 * Defines the vector struct and declares all of the associated functions.
 *
 * The 'how' field is a list of keywords to place in front of the function
 * declarations such as 'static inline', 'static', or 'extern'. In C99 and C11,
 * this field can be left blank, but C89 does not allow this behavior and you
 * will need to place 'extern' in this field.
 *
 * The 'namespace' argument allows you to customize the names of the type and
 * the functions. The defined type will always be 'namespace_t' and the
 * functions will be of the format 'namespace_method'.
 *
 * The 'type' field is the underlying type of the vector.
 */
#define VECTOR_DECLARE(how, namespace, type) \
	_VECTOR_DO_DECLARE(how, namespace, type, namespace ## _t)

/* Define
 *
 * Defines the functions declared by VECTOR_DECLARE. The arguments to this macro
 * are the same as the ones in VECTOR_DECLARE.
 */
#define VECTOR_DEFINE(how, namespace, type) \
	_VECTOR_DO_DEFINE(how, namespace, type, namespace ## _t)

/* Declare and Define
 *
 * A shortcut which calls VECTOR_DECLARE and VECTOR_DEFINE.
 */
#define VECTOR_DECLARE_AND_DEFINE(how, namespace, type) \
	VECTOR_DECLARE(how, namespace, type) \
	VECTOR_DEFINE(how, namespace, type)

/*
 * Initialize a statically allocated vector.
 */
#define VECTOR_INITIALIZER { 0, 0, NULL }

#endif /* __VECTOR_H__ */
