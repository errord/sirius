/*
Copyright (c) 1998, 1999, 2000 Thai Open Source Software Center Ltd
Portions Copyright (c) 1999 Netscape Communications Corporation.
See the file COPYING for copying permission.
*/

/* define memory allocate */
#define MALLOC CLIBMALLOC
#define FREE CLIBFREE
#define CALLOC CLIBCALLOC

typedef const char *KEY;

typedef struct {
  KEY name;
} NAMED;

typedef struct {
  NAMED **v;
  size_t size;
  size_t used;
  size_t usedLim;
} HASH_TABLE;

NAMED *lookup(HASH_TABLE *table, KEY name, size_t createSize);
void hashTableInit(HASH_TABLE *);
void hashTableDestroy(HASH_TABLE *);

typedef struct {
  NAMED **p;
  NAMED **end;
} HASH_TABLE_ITER;

void hashTableIterInit(HASH_TABLE_ITER *, const HASH_TABLE *);
NAMED *hashTableIterNext(HASH_TABLE_ITER *);
