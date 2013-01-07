#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../libcomm/libtime.h"
#include "wordbase.h"
#include "libdatrie.h"

struct datrietree_s* makeDatrieTree(int encodesize, struct datrieevent_s* event)
{
  struct datrietree_s* datrie;
  
  datrie = (struct datrietree_s*)LDMALLOC(sizeof(struct datrietree_s));
  if (datrie == NULL)
    {
      LDMEMOUT;
      LDMEMOUT_EXIT(NULL);
    }
  datrie->event = event;
  datrie->wordimage = create_wordimage(encodesize);
  if (datrie->wordimage == NULL)
    {
      LDMEMOUT;
      LDMEMOUT_EXIT(NULL);
    }
  datrie->trie = create_trie(datrie->wordimage);
  if (datrie->trie == NULL)
    {
      LDMEMOUT;
      LDMEMOUT_EXIT(NULL);
    }
  datrie->datrie = NULL;
  
  return datrie;
}

void clearDatrieTree(struct datrietree_s* datrie)
{
  if (datrie != NULL)
    {
      if (datrie->datrie != NULL)
        {
          clear_datrie(datrie->datrie);
          datrie->datrie = NULL;
        }
      if (datrie->trie != NULL)
        {
          clear_trie(datrie->trie);
          datrie->trie = NULL;
        }
      if (datrie->wordimage != NULL)
        {
          clear_wordimage(datrie->wordimage);
          datrie->wordimage = NULL;
        }
      datrie->event = NULL;
      LDFREE(datrie);
      datrie = NULL;
    }  
}

void addCnWord(struct datrietree_s* datrie, const char* word, struct userdata_s* userdata)
{
  struct tstate_s tstate;
  int* words;
  int wlen;
  int wcount;
  
  assert(datrie != NULL);
  assert(word != NULL);
  
  wlen = strlen(word);
  words = (int*)LDMALLOC(sizeof(int) * wlen);
  if (words == NULL)
    {
      LDMEMOUT;
      LDMEMOUT_EXIT(NULL);
    }
  
  wcount = wordcodebywordstring(word, wlen, words);
  tstate.states = words;
  tstate.statecount = wcount;
  /* copy user data */
  COPY_UD__P(tstate.userdata, userdata);
  add_states(datrie->trie, &tstate);
  LDFREE(words);
}

void buildDatrie(struct datrietree_s* datrie, int scantype)
{
  struct trie_state_s* state_list;
  
  assert(datrie->datrie == NULL);

  datrie->datrie = create_datrie(datrie->wordimage, datrie->event, get_userdata_count(datrie->trie), scantype);
  if (datrie->datrie == NULL)
    {
      LDMEMOUT;
      LDMEMOUT_EXIT(NULL);
    }
  state_list = get_sort_state_list(datrie->trie);
  build_state_list(datrie->datrie, state_list);
}

int findCnWord(struct datrietree_s* datrie, const char* word, struct userdata_s* userdata)
{
  struct tstate_s tstate;
  int* words;
  int wlen;
  int wcount;
  int r;
  
  assert(datrie != NULL);
  assert(word != NULL);
  
  wlen = strlen(word);
  words = (int*)LDMALLOC(sizeof(int) * wlen);
  if (words == NULL)
    {
      LDMEMOUT;
      LDMEMOUT_EXIT(NULL);
    }
  
  wcount = wordcodebywordstring(word, wlen, words);
  tstate.states = words;
  tstate.statecount = wcount;
  CLEAR_UD(tstate.userdata);
  r = dat_find_states(datrie->datrie, &tstate);
  LDFREE(words);
  COPY_UD_P_(userdata, tstate.userdata);
  return r;
}
