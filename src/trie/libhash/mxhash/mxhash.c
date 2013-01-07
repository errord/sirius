#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "hashtable.h"
#include "mxhash.h"

size_t mx_create_hashtable(unsigned int hashtable_size)
{
  HASH_TABLE *hash;
  
  hash = (HASH_TABLE*)malloc(sizeof(HASH_TABLE));
  hashTableInit(hash);
  return (size_t)hash;
}

void mx_destroy_hashtable(size_t ht)
{
  HASH_TABLE *hash;
  
  hash = (HASH_TABLE*)ht;
  hashTableDestroy(hash);
  free(hash);
}

void mx_ht_clear(size_t ht)
{
  HASH_TABLE *hash;
  
  hash = (HASH_TABLE*)ht;
  hashTableDestroy(hash);
  hashTableInit(hash);
}

void mx_ht_set(size_t ht, const char *key, unsigned int value)
{
  NAMED *named;
  HASH_TABLE *hash;
  
  hash = (HASH_TABLE*)ht;  
  named = lookup(hash, key, sizeof(NAMED) + sizeof(unsigned int));
  if (named == NULL)
    return;
  *((unsigned int*)(named+1)) = value;
}

unsigned int mx_ht_get(size_t ht, const char *key)
{
  NAMED *named;
  HASH_TABLE *hash;
  
  hash = (HASH_TABLE*)ht;  
  named = lookup(hash, key, sizeof(NAMED) + sizeof(unsigned int));
  if (named == NULL)
    return 0;
  return *((unsigned int*)(named+1));
}

