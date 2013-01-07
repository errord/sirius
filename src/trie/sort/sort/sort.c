#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "/home/error.d/work/clib/libcomm/libtime.h"
#include "../data/data.h"

void Counting_Sort(int a[], int b[], int l, int k)
{
  int j;
  int memsize = sizeof(int)*k;
  unsigned char* c = (unsigned char*)malloc(memsize);
  printf("memsize:%d\n", memsize);

  memset(c, 0, k * sizeof(int));

  for (j = 0; j < l; j++) c[a[j]]++;
  for (j = 1; j < k; j++) c[j] += c[j - 1];
  for (j = l - 1; j >= 0; j--)
    {
      b[c[a[j]] - 1] = a[j];
      c[a[j]]--;
    }
  free(c);
}

void quick_sort(int a[],int left,int right)
{
  int tmp;
  tmp = a[(left + right) / 2];
  int i = left;
  int j = right;
  int t;
  
  do{
    while(a[i] < tmp && i < right) i++;
    while(a[j] > tmp && j > left) j--;
    
    if(i <= j)
      {
        t = a[i];
        a[i] = a[j];
        a[j] = t;
        i++;
        j--;
      }
    
  }while(i <= j);

  if(left < j) quick_sort(a, left, j);
  if(i < right) quick_sort(a, i, right);
    
}

void heapAdjust(int a[], int s, int m)
{
  //已知a[s..m]中除a[s]之外均满足堆的定义,本函数调整a[s]
  //使a[s..m]成为一个大顶堆
  int j,rc;
  
  rc=a[s];
  
  for(j = 2*s; j <= m; j *= 2) //沿关键字叫大的结点向下筛选
    {
      // 选出大儿子 
      if(j < m && a[j] < a[j+1]) ++j;
      // 新来的是否比大儿子大?大就能当爹啦
      if(rc >= a[j]) break;
      // 太小,当不了新爹,换位,继续向下找合适的儿子
      a[s]=a[j];
      s=j;
      
    }
  a[s]=rc;
  //插入
}

void heap_sort(int a[], int l)
{
  
  //对顺序表L做堆排序。
  int i,j,t;
  
  for(i = l/2; i >= 0; --i) //把a[1..L.length]建成大顶堆
    heapAdjust(a, i, l);
  
  for(i = l; i > 0; --i)
    {
      
      t = a[0];
      //将堆顶记录和当前未经排序子序列a[1..i]
      a[0] = a[i];
      //中的最后一个记录相互交换
      a[i]=t;
      
      heapAdjust(a, 0, i-1);
      //将L.r[1..i-1]重新调整为大顶堆
    }  
}

void push_heap(int a[], int e)
{  
  int p;
  int t = 0;

  // 上溯
  while ((p = e / 2) > 0)
    {
      if (a[e-1] > a[p-1])
        {
          t = a[p-1];
          a[p-1] = a[e-1];
          a[e-1] = t;
        }
      e = p;
    }  
}

void pop_heap(int a[], int e)
{
  int p;
  int end;
  int h = 1;

  // 最大元素放入顺序位置
  end = a[h-1];
  
  // 回溯到叶子节点
  while ((p = h * 2) < e)
    {
      if (p+1 > e)
        {
          a[h-1] = a[p-1];
          break;
        }
      if (a[p-1] > a[p])
        {
          a[h-1] = a[p-1];
          h = p;
        }
      else
        {
          a[h-1] = a[p];
          h = p+1;
        }
    }
  // 填充空位
  a[h-1] = a[e-1];
  //上溯
  push_heap(a, p/2);
  a[e-1] = end;
}


void max_heap_sort(int a[], int l)
{
  int i;

  // 构造heap
  for (i = 1; i <= l; i++)
    push_heap(a, i);
  // 序列化
  for (i = l; i > 0; i--)
    pop_heap(a, i);
}


void place_sort(int a[], int b[], int l, long long k)
{
  int i;
  int j;
  int jj;
  long long memsize = sizeof(unsigned char)*k;
  unsigned char* c = (unsigned char*)malloc(memsize);
  printf("memsize:%d\n", memsize);
  memset(c, 0, k * sizeof(unsigned char));
  for (j = 0; j < l; j++)
    {
      c[a[j]]++; 
    }

  i = 0;
  for (j = 0; j < k; j++)
    {
      for (jj = 0; jj < c[j]; jj++)
        {
          b[i] = j;
          i++;
        }
      
    }
  free(c);
}

void bit_sort(const int *in, int *out, int len, int max)
{
  int i;
  char *bits;
  int bitcount;
  int count = 0;
  int outindex = 0;

  bitcount = max / 8;
  bitcount += (max % 8 > 0 ? 1 : 0);
  printf("maxnum:%d\n", max);
  printf("memsize:%d\n", bitcount);
  bits = (char*)malloc(sizeof(char) * bitcount);
  
  memset(bits, 0, bitcount * sizeof(char));
  for (i = 0; i < len; i++)
    {
      if (in[i] == 0)
        continue;
      
      int bi = in[i] / 8;
      char *c = &bits[bi];
      switch (in[i]%8)
        {
        case 0:
          *c |= 0x80; break;
        case 1:
          *c |= 0x40; break;
        case 2:
          *c |= 0x20; break;
        case 3:
          *c |= 0x10; break;
        case 4:
          *c |= 0x8; break;
        case 5:
          *c |= 0x4; break;
        case 6:
          *c |= 0x2; break;
        case 7:
          *c |= 0x1; break;
        }
    }
  for (i = 0; i < bitcount; i++)
    {
      char c = bits[i];
      int n;
      unsigned char bit = 0x80;
      
      
      if (c == 0)
        {
          count += 8;
          continue;
        }
      

      for (n = 0; n < 8; n++)
        {
          if ((c&bit) == bit)
            {
              out[outindex] = count;
              outindex++;
            }
          count++;
          bit >>= 1;
        }
    }
  
  free(bits);
}

void insert_sort(int a[], int n)
{    
  int i;
  int j;
  int tmp;

  for (i = 0; i < n; ++i)
    {
      
      tmp = a[i];
      // copy it first
      for (j = i; j > 0; --j) // unsorted region; (0 ~ (i - 1)) is sorted
        if (a[j - 1] > tmp)
          a[j] = a[j - 1];
      // move back elements to empty a right position
        else
          break;
      // we got it! x[j] is the right position
      a[j] = tmp;
      // place it to the right position
    }
}


void outArray(int a[], unsigned int l)
{
  int i;

  for(i = 0; i < l; i++)
    {
      printf("%d,", a[i]);
    }
}

int main()
{
  int *a = NULL;
  int *b = NULL;
  time_info ti1;
  time_info ti2;
  int c = 0;
  char* datafile = "./data";
  sort_data* sd = NULL;
  int max;
  int count;
  
  sd = load_sort_data(datafile);
  count = count_sort_data(sd);
  max = max_sort_data(sd);

  printf("sort data count: %d\n", count);
  printf("sort data max: %d\n", max);
  max++;
  
  a = data_sort_data(sd);
  b = (int*)malloc(sizeof(int) * count);
  
  printf("sort data:\n");
  //  outArray(a, count);
  printf("\n\n");

  printf("counting sort ...\n");
  timestart(&ti1);
  Counting_Sort(a, b, count, max);
  c = timeend(&ti1);
  printf("time:%d\n", c);
  //  outArray(b, count);
  printf("\n\n");

  printf("place sort ...\n");
  timestart(&ti2);
  place_sort(a, b, count, max);
  c = timeend(&ti2);

  printf("time:%d\n", c);
  // outArray(b, count);
  //tofile_sort_data(b, count, "place_sort_data");
  printf("\n");

  printf("bit sort ...\n");
  timestart(&ti2);
  bit_sort(a, b, count, max);
  c = timeend(&ti2);

  printf("time:%d\n", c);
  // outArray(b, count);
  //tofile_sort_data(b, count, "bin_sort_data");
  printf("\n");


  printf("quick sort ...\n");
  timestart(&ti2);
  quick_sort(a, 0, count-1);
  c = timeend(&ti2);

  printf("time:%d\n", c);
  // outArray(b, count);
  //tofile_sort_data(a, count, "quick_sort_data");
  printf("\n");

  unload_sort_data(sd);

  // heap sort
  sd = load_sort_data(datafile);
  count = count_sort_data(sd);
  max = max_sort_data(sd);
  max++;
  
  a = data_sort_data(sd);

  printf("heap sort ...\n");
  timestart(&ti2);
  heap_sort(a, count-1);
  c = timeend(&ti2);

  printf("time:%d\n", c);
  //outArray(b, count);
  tofile_sort_data(a, count, "heap_sort_data");
  printf("\n");

  unload_sort_data(sd);

  // max heap sort
  sd = load_sort_data(datafile);
  count = count_sort_data(sd);
  max = max_sort_data(sd);
  max++;
  
  a = data_sort_data(sd);

  printf("max heap sort ...\n");
  timestart(&ti2);
  max_heap_sort(a, count);
  c = timeend(&ti2);
  printf("time:%d\n", c);
  timestart(&ti2);
  push_heap(a, 2345234);
  push_heap(a, 2344);
  push_heap(a, 1232344);
  push_heap(a, 25234);
  push_heap(a, 235234);
  push_heap(a, 23445234);
  push_heap(a, 65434234);
  push_heap(a, 12323434);
  push_heap(a, 25234);
  push_heap(a, 235234);
  c = timeend(&ti2);
  printf("insert 10 to max heap time:%d\n", c);
  //outArray(b, count);
  tofile_sort_data(a, count, "max_heap_sort_data");
  printf("\n");

  unload_sort_data(sd);

  // insert sort
  sd = load_sort_data(datafile);
  count = count_sort_data(sd);
  max = max_sort_data(sd);
  max++;
  
  a = data_sort_data(sd);

  printf("insert sort ...\n");
  timestart(&ti2);
  insert_sort(a, count-1);
  c = timeend(&ti2);

  printf("time:%d\n", c);
  //outArray(b, count);
  tofile_sort_data(a, count, "insert_sort_data");
  printf("\n");

  unload_sort_data(sd);


  return 0;
}
