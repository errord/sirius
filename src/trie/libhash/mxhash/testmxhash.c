#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"

int main()
{
  int ht;
  int a;
  int b;
  char* c;
  char* d;

  ht = mx_create_hashtable(100);
  mx_ht_set(ht, "a", 1);
  mx_ht_set(ht, "b", 2);
  a = mx_ht_get(ht, "a");
  b = mx_ht_get(ht, "b");
  printf("a=%d\n", a);
  printf("b=%d\n", b);
  mx_ht_set(ht, "a", 10);
  a = mx_ht_get(ht, "a");
  b = mx_ht_get(ht, "b");
  c = (char*)mx_ht_get(ht, "c");
  d = (char*)mx_ht_get(ht, "国家叫什么啊?");
  printf("a=%d\n", a);
  printf("b=%d\n", b);
  printf("c=%s\n", c);
  printf("国家叫什么啊?=%s\n", d);
  mx_destroy_hashtable(ht);
  return 0;
}
