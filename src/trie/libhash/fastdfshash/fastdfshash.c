#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "hash.h"
#include "fastdfshash.h"

struct _hash
{
  HashArray hash;
  unsigned int size;
};

size_t fdhash_create_hashtable(unsigned int hashtable_size)
{
  struct _hash *hash;
  
  hash = (struct _hash*)malloc(sizeof(struct _hash));
  hash_init(&hash->hash, RSHash, hashtable_size, 5);
  hash->size = hashtable_size;
  return (size_t)hash;
}

void fdhash_destroy_hashtable(size_t ht)
{
  struct _hash *hash;
  
  hash = (struct _hash*)ht;
  hash_destroy(&hash->hash);
  free(hash);
}

void fdhash_ht_clear(size_t ht)
{
  struct _hash *hash;
  
  hash = (struct _hash*)ht;
  hash_destroy(&hash->hash);
  hash_init(&hash->hash, RSHash, hash->size, 5);
}

void fdhash_ht_set(size_t ht, const char *key, size_t value)
{
  struct _hash *hash;
  
  hash = (struct _hash*)ht;
  hash_insert(&hash->hash, (const void*)key, strlen(key), (void*)value);
}

size_t fdhash_ht_get(size_t ht, const char *key)
{
  void* value;
  struct _hash *hash;
  
  hash = (struct _hash*)ht;
  value = hash_find(&hash->hash, (const void*)key, strlen(key));
  return (size_t)value;
}

