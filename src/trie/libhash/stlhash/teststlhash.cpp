#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stlhash.h"

int main()
{
  int ht;
  int a;
  int b;
  char* c;
  char* d;
  
  ht = stl_create_hashtable(100);
  stl_ht_set(ht, "a", 1);
  stl_ht_set(ht, "b", 2);
  a = stl_ht_get(ht, "a");
  b = stl_ht_get(ht, "b");
  printf("a=%d\n", a);
  printf("b=%d\n", b);
  stl_ht_set(ht, "a", 10);
  a = stl_ht_get(ht, "a");
  b = stl_ht_get(ht, "b");
  c = (char*)stl_ht_get(ht, "c");
  d = (char*)stl_ht_get(ht, "国家叫什么啊?");
  printf("a=%d\n", a);
  printf("b=%d\n", b);
  printf("c=%s\n", c);
  printf("国家叫什么啊?=%s\n", d);
  stl_destroy_hashtable(ht);
  return 0;
}
