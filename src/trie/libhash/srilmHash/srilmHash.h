/*
 * clib -- libhash srilm hash
 */
#ifndef _CLIB_LIBHASH_SRILMHASH_H_
#define _CLIB_LIBHASH_SRILMHASH_H_

size_t sri_create_hashtable(unsigned int hashtable_size);
void sri_destroy_hashtable(size_t ht);
void sri_ht_clear(size_t ht);
void sri_ht_set(size_t ht, const char* key, unsigned int value);
unsigned int sri_ht_get(size_t ht, const char* key);

#endif /* _CLIB_LIBHASH_SRILMHASH_H_ */
