#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fastdfshash.h"

int main()
{
  int ht;
  int a;
  int b;
  char* c;
  char* d;
  
  ht = fdhash_create_hashtable(100);
  fdhash_ht_set(ht, "a", 1);
  fdhash_ht_set(ht, "b", 2);
  a = fdhash_ht_get(ht, "a");
  b = fdhash_ht_get(ht, "b");
  printf("a=%d\n", a);
  printf("b=%d\n", b);
  fdhash_ht_set(ht, "a", 10);
  a = fdhash_ht_get(ht, "a");
  b = fdhash_ht_get(ht, "b");
  c = (char*)fdhash_ht_get(ht, "c");
  d = (char*)fdhash_ht_get(ht, "国家叫什么啊?");
  printf("a=%d\n", a);
  printf("b=%d\n", b);
  printf("c=%s\n", c);
  printf("国家叫什么啊?=%s\n", d);
  fdhash_destroy_hashtable(ht);
  return 0;
}
