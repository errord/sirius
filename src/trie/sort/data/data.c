#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "data.h"


static void add_number(sort_data* sd, int number)
{
  if (sd == NULL)
    return ;
  
  sd->count++;
  if (sd->count > sd->size)
    {
      sd->size += DEF_SIZE;
      sd->data = realloc(sd->data, sd->size);
    }

  sd->data[sd->count-1] = number;
}


sort_data* load_sort_data(char* datafilename)
{
  sort_data* sd = NULL;
  FILE* fd = NULL;
  char buff[BUFF_SIZE];
  char nbuf[BUFF_SIZE];
  int number;
  int rl = 0;
  int ri = 0;
  int bi = 0;
  
  if (datafilename == NULL)
    return NULL;

  sd = (sort_data*)malloc(sizeof(sort_data));
  sd->size = DEF_SIZE;
  sd->data = (int*)malloc(sizeof(int) * sd->size);
  sd->count = 0;
  
  fd = fopen(datafilename, "r");
  if (fd == NULL)
    {
      printf("open file %s error.\n", datafilename);
      exit(0);
    }
  
  while ((rl = fread(buff, sizeof(char), BUFF_SIZE, fd)) > 0)
    {
      for (ri = 0; ri < rl; ri++)
        {
          if (buff[ri] == ',')
            {
              nbuf[bi] = '\0';
              number = atoi(nbuf);
              add_number(sd, number);
              bi = 0;
              number = 0;
            }
          else
            {
              nbuf[bi] = buff[ri];
              bi++;
            } 
        }
    }
  if (bi != 0)
    {
      nbuf[bi+1] = '\0';
      number = atoi(nbuf);
      add_number(sd, number);
    }
  
  return sd;
}

void unload_sort_data(sort_data* sd)
{
  sd->size = 0;
  sd->count = 0;
  free(sd->data);
}

int* data_sort_data(sort_data* sd)
{
  return sd->data;
}


int count_sort_data(sort_data* sd)
{
  return sd->count;
}

int max_sort_data(sort_data* sd)
{
  int max = 0;
  int count = sd->count;
  
  while (count--)
    {
      if (sd->data[count] > max)
        max = sd->data[count];
    }
  return max;
}

void tofile_sort_data(int sort_data[], int l, const char* file)
{
  FILE* fd = NULL;
  int i;
  char buff[BUFF_SIZE];
  
  fd = fopen(file, "w");
  for (i = 0; i < l; i++)
    {
      sprintf(buff, "%d,", sort_data[i]);
      fwrite(buff, sizeof(char), strlen(buff), fd);
    }
  fclose(fd);
}
