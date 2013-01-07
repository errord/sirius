#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <sarray.h>

#define USE_STACK(s, size) (size <= s->stacksize)
#define NO_USE_STACK(s, size) (!USE_STACK(s, size))
#define BUF_USE_STACK(s) (s->bufsize <= s->stacksize)
#define BUF_NO_USE_STACK(s) (!BUF_USE_STACK(s))

#define copy_elem(dest, src, n) memcpy(dest, src, sizeof(size_t) * n)
#define set_elem(dest, elem, n) memset(dest, elem, sizeof(size_t) * n)

static size_t *_alloc(sarray_s *array, size_t size)
{
	if (size == 0) {
		array->element = NULL;
	} else if (NO_USE_STACK(array, size)) {
		array->element = (size_t*)malloc(sizeof(size_t) * size);
	} else {
		array->element = array->stackbuf;
	}
	array->len = 0;
	array->bufsize = size;
	return array->element;
}

static void _free(sarray_s *array)
{
	if (BUF_NO_USE_STACK(array))
		free(array->element);
	array->element = NULL;
}

sarray_s *sarray_new(size_t bufsize)
{
	sarray_s *array;
	array = (sarray_s*)malloc(sizeof(sarray_s));
	if (array == NULL)
		return NULL;
	array->stacksize = bufsize;
	_alloc(array, bufsize);
	return array;
}

sarray_s *sarray_stacksize_new(size_t stacksize, size_t bufsize)
{
	sarray_s *array;
	size_t size = sizeof(sarray_s) - sizeof(size_t) * SARRAY_STACKBUF_MAX + sizeof(size_t) * stacksize;
	array = (sarray_s*)malloc(size);
	if (array == NULL)
		return NULL;
	array->stacksize = stacksize;
	_alloc(array, bufsize);
	return array;
}

void sarray_resource_init(sarray_s *array)
{
	array->element = NULL;
	array->len = 0;
	array->bufsize = 0;
	array->stacksize = SARRAY_STACKBUF_MAX;
}

void sarray_resource_clear(sarray_s *array)
{
	if (array->element != NULL)
		_free(array);
	array->element = NULL;
	array->len = 0;
	array->bufsize = 0;
}

void sarray_free(sarray_s *array)
{
	sarray_resource_clear(array);
	free(array);
}

size_t *sarray_realloc_empty(sarray_s *array, size_t size)
{
	sarray_resource_clear(array);
	return _alloc(array, size);
}

size_t *sarray_realloc(sarray_s *array, size_t size)
{
	size_t *ptr;
	size_t len;
	if (size == 0) {
		_free(array);
		len = 0;
		ptr = NULL;
	} else if (NO_USE_STACK(array, size)) {
		ptr = (size_t*)malloc(sizeof(size_t) * size);
		len = size > array->len ? array->len : size;
		if (len > 0)
			copy_elem(ptr, array->element, len);
		_free(array);
	} else {
		/* new length */
		len = size > array->len ? array->len : size;
		/* old array no use stack , copy to stack */
		if (BUF_NO_USE_STACK(array)) {
			if (len > 0)
				copy_elem(array->stackbuf, array->element, len);
		}
		_free(array);
		ptr = array->stackbuf;
	}
	array->element = ptr;
	array->len = len;
	array->bufsize = size;
	return array->element;
}

void sarray_set_safe(sarray_s *array, size_t idx, size_t elem)
{ 
	size_t bufsize;
	size_t expand;
	size_t *ptr;
	/* expand element room */
	if (idx >= sarray_bufsize_m(array)) {
		bufsize = idx + 1;
		bufsize <<= 1; /* X2 */
		sarray_realloc(array, bufsize);
	}
	/* expand element set 0 */
	if (idx >= sarray_len_m(array)) {
		expand = idx - sarray_len_m(array) + 1;
		ptr = array->element + sarray_len_m(array);
		set_elem(ptr, 0, expand);
		array->len = idx+1;
	} 
	array->element[idx] = elem;
}

size_t sarray_add_safe(sarray_s *array, size_t elem)
{
	sarray_set_safe(array, sarray_len_m(array), elem);
	return sarray_len_m(array) - 1;
}

size_t sarray_get_safe(sarray_s *array, size_t idx)
{
	if (idx >= sarray_len_m(array))
		sarray_set_safe(array, idx, 0);
	return array->element[idx];
}

int sarray_find(sarray_s *array, size_t elem, size_t *idx)
{
	size_t e;
	sarray_each_m(e, array) {
		if (e == elem) {		
			*idx = dss;
			return 1;
		}
	}
	sarray_each_end_m;
	return 0;
}

size_t sarray_add_uniquely_safe(sarray_s *array, size_t elem)
{
	size_t idx;
	if (!sarray_find(array, elem, &idx))
		sarray_add_safe(array, elem);
	return sarray_len_m(array);
}

/* TODO: optimize */
size_t sarray_merge_union(sarray_s *array1, sarray_s *array2)
{
	size_t e;
	size_t idx;
	sarray_each_m(e, array2) {
		if (!sarray_find(array1, e, &idx))
			sarray_add_safe(array1, e);
	}
	sarray_each_end_m;
	return sarray_len_m(array1);
}
