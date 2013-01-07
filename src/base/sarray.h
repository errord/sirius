#ifndef _S_ARRAY_H_
#define _S_ARRAY_H_

/*
 * simpleness array 
*/

#define SARRAY_STACKBUF_MAX 1024

typedef struct {
	size_t *element;
	size_t len;
	size_t bufsize;
	size_t stacksize;
	size_t stackbuf[SARRAY_STACKBUF_MAX];
} sarray_s;

sarray_s *sarray_new(size_t bufsize);
sarray_s *sarray_stacksize_new(size_t stacksize, size_t bufsize);
void sarray_resource_init(sarray_s *array);
/* clear array resource */
void sarray_resource_clear(sarray_s *array);
/* clear array resource and array self */
void sarray_free(sarray_s *array);
size_t *sarray_realloc_empty(sarray_s *array, size_t size);
size_t *sarray_realloc(sarray_s *array, size_t size);
/* set element to array, realloc if idx >= buff size */
void sarray_set_safe(sarray_s *array, size_t idx, size_t elem);
/* add element at end, return end index realloc if idx >= buff size*/
size_t sarray_add_safe(sarray_s *array, size_t elem);
/* add uniquely element at end, return end index realloc if idx >= buff size*/
size_t sarray_add_uniquely_safe(sarray_s *array, size_t elem);
/* get element to array, realloc if idx >= buff size */
size_t sarray_get_safe(sarray_s *array, size_t idx);
/* find element, return 0 if not exist, other return 1, idx is element index*/
int sarray_find(sarray_s *array, size_t elem, size_t *idx);
/* union of set merge array1 and array2 to array1 */
size_t sarray_merge_union(sarray_s *array1, sarray_s *array2);


#define sarray_len_m(array) ((array)->len)
#define sarray_bufsize_m(array) ((array)->bufsize)
#define sarray_stacksize_m(array) ((array)->stacksize)
#define sarray_set_m(array, idx, elem) ((array)->element[idx] = elem)
#define sarray_add_m(array, elem) { (array)->element[sarray_len_m(array)] = elem; sarray_len_m(array)++; }
#define sarray_get_m(array, idx) ((array)->element[idx])
#define sarray_elem_exist_m(array, idx) (idx < sarray_len_m(array))
#define sarray_each_m(elem, array) {size_t dss;for(dss=0;dss<sarray_len_m(array);++dss){(elem=array->element[dss]);
#define sarray_each_type_m(elem, type, array) {size_t dss;for(dss=0;dss<sarray_len_m(array);++dss){(elem=(type)(array->element[dss]));
#define sarray_each_ptr_m(elem_ptr, array) {size_t dss;for(dss=0;dss<sarray_len_m(array);++dss){(elem_ptr=&(array->element[dss]));
#define sarray_each_end_m }}
#define sarray_each_type_end_m }}

#endif /* _S_ARRAY_H_ */
