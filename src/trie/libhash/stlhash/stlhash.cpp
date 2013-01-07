#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "stlhash.h"

size_t stl_create_hashtable(unsigned int hashtable_size)
{
  std::map<const char*, size_t>* hash;
  
  hash = new std::map<const char*, size_t>;
  return (size_t)hash;
}

void stl_destroy_hashtable(size_t ht)
{
  std::map<const char*, size_t>* hash;

  hash = (std::map<const char*, size_t>*)ht;
  delete hash;
}

void stl_ht_clear(size_t ht)
{
  std::map<const char*, size_t>* hash;

  hash = (std::map<const char*, size_t>*)ht;
  hash->clear();
}

void stl_ht_set(size_t ht, const char *key, unsigned int value)
{
  std::map<const char*, size_t>* hash;

  hash = (std::map<const char*, size_t>*)ht;
  (*hash)[key] = value;
}

unsigned int stl_ht_get(size_t ht, const char *key)
{
  std::map<const char*, size_t>* hash;

  hash = (std::map<const char*, size_t>*)ht;
  return (*hash)[key];
}

