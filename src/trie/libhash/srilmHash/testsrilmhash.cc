#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "srilmHash.h"

int main()
{
  int ht;
  int a;
  int b;
  char* c;
  char* d;
  
  ht = sri_create_hashtable(100);
  sri_ht_set(ht, "a", 1);
  sri_ht_set(ht, "b", 2);
  a = sri_ht_get(ht, "a");
  b = sri_ht_get(ht, "b");
  printf("a=%d\n", a);
  printf("b=%d\n", b);
  sri_ht_set(ht, "a", 10);
  a = sri_ht_get(ht, "a");
  b = sri_ht_get(ht, "b");
  c = (char*)sri_ht_get(ht, "c");
  d = (char*)sri_ht_get(ht, "国家叫什么啊?");
  printf("a=%d\n", a);
  printf("b=%d\n", b);
  printf("c=%s\n", c);
  printf("国家叫什么啊?=%s\n", d);
  sri_destroy_hashtable(ht);
  return 0;
}
