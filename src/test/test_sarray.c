#include <stdlib.h>
#include <stdio.h>
#include <easycheck.h>
#include <sarray.h>

int main()
{
	sarray_s *array;
	size_t v;
	size_t *vp;
	size_t check_array[10] = {
		10, 23, 342, 234, 23, 0, 23, 123, 0, 3
	};
	char *info = "Sarray";
	echeck_start(info);
	array = sarray_new(2048);
	/* case */
	v = sarray_get_safe(array, 1000);
	ECHECK("sarray_get_safe(1)", v == 0);
	/* case */
	v = sarray_get_m(array, 999);
	ECHECK("sarray_get_safe(2)", v == 0);
	/* case */
	sarray_set_safe(array, 3325, 6);
	v = sarray_get_m(array, 3325);
	ECHECK("sarray_set_safe(1)", v == 6);
	/* case */
	sarray_set_safe(array, 988, 8);
	v = sarray_get_m(array, 988);
	ECHECK("sarray_set_safe(2)", v == 8);
	ECHECK("sarray_bufsize", sarray_bufsize_m(array) == 3326*2);
	ECHECK("sarray_len", sarray_len_m(array) == 3326);
	/* case */
	sarray_set_m(array, 0, 10);
	sarray_set_m(array, 1, 23);
	sarray_set_m(array, 2, 342);
	sarray_set_m(array, 3, 234);
	sarray_set_m(array, 4, 23);
	sarray_set_m(array, 5, 0);
	sarray_set_m(array, 6, 23);
	sarray_set_m(array, 7, 123);
	sarray_set_m(array, 8, 0);
	sarray_set_m(array, 9, 3);
	sarray_realloc(array, 10);
	ECHECK("sarray_realloc", sarray_len_m(array) == 10 && sarray_bufsize_m(array) == 10);
	/* case */
	sarray_each_m(v, array) {
		ECHECK("sarray_realloc\tsarray_get", v == check_array[dss]);
	}
	sarray_each_end_m;
	/* case */
	sarray_each_ptr_m(vp, array) {
		(*vp)++;
	}
	sarray_each_end_m;
	sarray_each_m(v, array) {
		ECHECK("sarray_each_ptr", v == check_array[dss]+1);
	}
	sarray_each_end_m;
	/* case */
	sarray_realloc_empty(array, 10);
	ECHECK("sarray_realloc_empty", sarray_len_m(array) == 0 && sarray_bufsize_m(array) == 10);
	sarray_free(array);
	echeck_end();
	return 0;
}
