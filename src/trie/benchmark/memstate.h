#ifndef _BM_MEMSTATE_H_
#define _BM_MEMSTATE_H_

void *bm_malloc(size_t size);
void *bm_realloc(void *ptr, size_t size);
void *bm_calloc(size_t nmemb, size_t size);
void bm_free(void *ptr);

size_t bm_getmemsize();
size_t bm_getalloccount();
void bm_resetmemsize();

void* operator new(size_t size);
void operator delete(void* ptr);

void* operator new[](size_t size);
void operator delete[](void* ptr);

#endif /* _BM_MEMSTATE_H_ */
