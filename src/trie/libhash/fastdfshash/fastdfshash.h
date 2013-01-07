/*
 * clib -- libhash fast dfs hash
 */
#ifndef _CLIB_LIBHASH_FDHASH_H_
#define _CLIB_LIBHASH_FDHASH_H_

size_t fdhash_create_hashtable(unsigned int hashtable_size);
void fdhash_destroy_hashtable(size_t ht);
void fdhash_ht_clear(size_t ht);
void fdhash_ht_set(size_t ht, const char *key, size_t value);
size_t fdhash_ht_get(size_t ht, const char *key);

#endif /* _CLIB_LIBHASH_FDHASH_H_ */

