/*
 * clib -- libhash stl hash
 */
#ifndef _CLIB_LIBHASH_STLHASH_H_
#define _CLIB_LIBHASH_STLHASH_H_

size_t stl_create_hashtable(unsigned int hashtable_size);
void stl_destroy_hashtable(size_t ht);
void stl_ht_clear(size_t ht);
void stl_ht_set(size_t ht, const char *key, unsigned int value);
unsigned int stl_ht_get(size_t ht, const char *key);

#endif /* _CLIB_LIBHASH_STLHASH_H_ */
