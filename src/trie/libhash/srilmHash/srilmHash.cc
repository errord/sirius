#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "LHash.h"
#include "Map.cc"
#include "LHash.cc"

size_t sri_create_hashtable(unsigned int hashtable_size)
{
  LHash<const char *, unsigned int> *hash;
  
  hash = new LHash<const char *, unsigned int>;
  return (size_t)hash;
}

void sri_destroy_hashtable(size_t ht)
{
  LHash<const char *, unsigned int> *hash;
  
  hash = (LHash<const char *, unsigned int>*)ht;
  delete hash;
}

void sri_ht_clear(size_t ht)
{
  LHash<const char *, unsigned int> *hash;
  
  hash = (LHash<const char *, unsigned int>*)ht;
  unsigned int size = hash->numEntries();
  hash->clear(size);
}

void sri_ht_set(size_t ht, const char *key, unsigned int value)
{
  LHash<const char *, unsigned int> *hash;
  unsigned int *p;
  
  hash = (LHash<const char *, unsigned int>*)ht;
  p = hash->insert(key);
  *p = value;
}

unsigned int sri_ht_get(size_t ht, const char *key)
{
  LHash<const char *, unsigned int> *hash;
  unsigned int *p;
  
  hash = (LHash<const char *, unsigned int>*)ht;
  p = hash->find(key);
  if (p == NULL)
    return 0;
  return *p;
}

