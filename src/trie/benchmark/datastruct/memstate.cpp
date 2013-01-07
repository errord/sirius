#include "util.h"
#include "memstate.h"

static size_t allocsize = 0;
static size_t alloccount = 0;

void *bm_malloc(size_t size)
{
  allocsize += size;
  alloccount++;
  return malloc(size);
}

void *bm_realloc(void *ptr, size_t size)
{
  allocsize += size;
  alloccount++;
  return realloc(ptr, size);
}

void *bm_calloc(size_t nmemb, size_t size)
{
  allocsize += (nmemb * size);
  alloccount++;
  return calloc(nmemb, size);
}

void bm_free(void *ptr)
{
  free(ptr);
}

size_t bm_getmemsize()
{
  return allocsize;
}

size_t bm_getalloccount()
{
  return alloccount;
}

void bm_resetmemsize()
{
  allocsize = 0;
  alloccount = 0;
}

void* operator new(size_t size)
{
  return bm_malloc(size);
}

void operator delete(void* ptr)
{
  return bm_free(ptr);
}

void* operator new[](size_t size)
{
  return bm_malloc(size);
}

void operator delete[](void* ptr)
{
  return bm_free(ptr);
}
