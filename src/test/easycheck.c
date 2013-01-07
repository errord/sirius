#include <stdlib.h>
#include <stdio.h>
#include <easycheck.h>

static int check_cout = 0;
static int failed_count = 0;
static int success_count = 0;

void echeck_start(const char* info)
{
  check_cout = 0;
  failed_count = 0;
  success_count = 0;
  printf("Check Task Name:[%s]\n", info);
}


void echeck(const char* info, int b, const char* file, int line)
{
  check_cout++;
  if (b == 0)
    {
      printf("Check... %s [Failed %s:%d]\n", info, file, line);
      failed_count++;
    }
  else
    {    
      printf("Check... %s [Success]\n", info);
      success_count++;
    }
  
}

void echeck_end()
{
  printf("CheckCount:%d\tSucess:%d\tFailed:%d\n", check_cout, success_count, failed_count);
}

