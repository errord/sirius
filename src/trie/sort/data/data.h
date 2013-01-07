#ifndef _SORT_DATA_H_
#define _SORT_DATA_H_

struct sort_data_s
{
  int* data;
  int size;
  int count;
};
typedef struct sort_data_s sort_data;

#define BUFF_SIZE 1000
#define DEF_SIZE 100000000

sort_data* load_sort_data(char* datafilename);
void unload_sort_data(sort_data* sd);
int* data_sort_data(sort_data* sd);
int count_sort_data(sort_data* sd);
int max_sort_data(sort_data* sd);
void tofile_sort_data(int sort_data[], int l, const char* file);

#endif // _SORT_DATA_H_
