/*
 *
 * libdatrie
 *
 */
#ifndef _CLIB_LIBDATRIE_H_
#define _CLIB_LIBDATRIE_H_
#include "clib.h"
#include "wordimage.h"
#include "trie.h"
#include "datrie.h"
#include "check.h"
#include "datrieevent.h"

#define LDMEMOUT CLIBMEMOUT
#define LDMEMOUT_EXIT(ret) CLIBMEMOUT
/* define memory allocate */
#define LDMALLOC CLIBMALLOC
#define LDFREE CLIBFREE

struct datrietree_s
{
  struct wordimage_s* wordimage;
  struct trie_s* trie;
  struct datrie_s* datrie;
  struct datrieevent_s* event;
};

struct datrietree_s* makeDatrieTree(int encodesize, struct datrieevent_s* event);

void clearDatrieTree(struct datrietree_s* datrie);

void addCnWord(struct datrietree_s* datrie, const char* word, struct userdata_s* userdata);

void buildDatrie(struct datrietree_s* datrie, int scantype);

int findCnWord(struct datrietree_s* datrie, const char* word, struct userdata_s* userdata);

#include "dictdatrie.h"

#endif /* _CLIB_LIBDATRIE_H_ */
