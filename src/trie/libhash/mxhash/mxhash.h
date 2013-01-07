/*
 * clib -- libhash mozilla xmlparse hash
 */
#ifndef _CLIB_LIBHASH_MXHASH_H_
#define _CLIB_LIBHASH_MXHASH_H_

size_t mx_create_hashtable(unsigned int hashtable_size);
void mx_destroy_hashtable(size_t ht);
void mx_ht_clear(size_t ht);
void mx_ht_set(size_t ht, const char *key, unsigned int value);
unsigned int mx_ht_get(size_t ht, const char *key);

#endif /* _CLIB_LIBHASH_MXHASH_H_ */

