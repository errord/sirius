#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>



int main()
{
  FILE* fd = NULL;
  int i;
  int n = 100000000;

  srand((unsigned)time(NULL));  
  while(n--)
    {
      i = rand()%100000000;
      printf("%d,", i);
    }
  return 0;
  
}
