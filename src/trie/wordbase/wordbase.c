#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>
#include "wordbase.h"

struct words_s* create_result_word(int huge)
{
  struct words_s* words;
  
  words = (struct words_s*)MALLOC(sizeof(struct words_s));
  if (words == NULL)
    {
      MEMOUT;
    }
  words->wordcount = 0;
  words->wordssize = 0;
  words->curindex = 0;
  words->head = NULL;
  words->tail = NULL;
  words->hugenode = NULL;
  words->curnode = NULL;
  words->huge = huge;
  return words;
}

void clear_result_word(struct words_s* words)
{
  struct word_s* word;
  struct word_s* curword;
  
  if (words == NULL)
    return;
  word = words->head;
  while (word != NULL)
    {
      /* free word */
      if (word->word != NULL)
        {
          FREE(word->word);
          word->word = NULL;
        }
      word->wordlen = 0;
      curword = word;
      word = word->next;
      FREE(curword);
    }
  FREE(words);
}

void addresultword(struct words_s* words, char* word, int freq)
{
  struct word_s* freenode;
  int i;
  int len;
  
  if (words == NULL || word == NULL)
    return;
 
  /* words is full */
  if (words->wordcount == words->wordssize)
    {
      freenode = (struct word_s*)MALLOC(sizeof(struct word_s));
      freenode->word = NULL;
      freenode->wordlen = 0;
      freenode->freq = 0;
      freenode->next = NULL;
      freenode->hugelink = NULL;
      /* first word */
      if (words->head == NULL)
        {
          words->head = words->tail = freenode;
        }
      else
        /* link to tail */
        {
          words->tail->next = freenode;
          words->tail = freenode;
        }
      words->wordssize++;
    }
  else
    {
      freenode = words->head;
      /* move to free node */
      for (i = 0; i < words->wordcount; i++)
        {
          freenode = freenode->next;
        }
    }
  /* add word to node */
  len = strlen(word);
  if (len >= freenode->wordlen)
    {
      /* first node and hugenode word is empty */
      if (freenode->word != NULL)
        {
          FREE(freenode->word);
        }
      freenode->word = (char*)MALLOC(sizeof(char) * (len+1));
    }
  strcpy(freenode->word, word);
  freenode->wordlen = len;
  /* hugenode */
  if (len > words->huge)
    {
      freenode->hugelink = words->hugenode;
      words->hugenode = freenode;
    }
  freenode->freq = freq;
  words->wordcount++;
}

struct word_s* get_word_node(struct words_s* words)
{
  struct word_s* curnode;
  
  if (words == NULL)
    return NULL;
  
  if (words->curindex >= words->wordcount)
    return NULL;
  
  curnode = words->curnode;
  if (curnode != NULL)
    words->curnode = curnode->next;
  words->curindex++;
  return curnode;
}

void result_head_to_curnode(struct words_s* words)
{
  if (words == NULL)
    return ;
  words->curnode = words->head;
}


struct word_s* get_result_word_head(struct words_s* words)
{
  if (words == NULL)
    return NULL;
  return words->head;
}

int get_result_word_count(struct words_s* words)
{
  if (words == NULL)
    return 0;
  return words->wordcount;
}


void reset_result_words(struct words_s* words)
{
  struct word_s* hugenode;
  struct word_s* tmp;
  
  if (words == NULL)
    return;
  
  words->wordcount = 0;
  words->curindex = 0;
  words->curnode = NULL;
  hugenode = words->hugenode;
  while (hugenode != NULL)
    {
      assert(hugenode->word != NULL);
      FREE(hugenode->word);
      hugenode->word = NULL;
      hugenode->wordlen = 0;
      tmp = hugenode;
      hugenode = hugenode->hugelink;
      tmp->hugelink = NULL;
    }
}

int wordcodebywordstring(const char* word, int wlength, int* words)
{
  int wcount = 0;
  int i;
  
  for (i = 0; i < wlength; i++)
    {
      words[wcount] = 0;
      if (word[i] < 0)
        {
          words[wcount] = (word[i++] & 0x0FF);
          words[wcount] |= ((word[i] & 0x0FF) << 8);
        }
      else
        {
          words[wcount] = word[i];
        }
      wcount++;
    }
  return wcount;
}

char* wordstringbywordcode(int* wordcode, char* wordstr, int wcs, int wce)
{
  int m = 0;
  int n;
  for (n = wcs; n < wce; n++)
    {
      wordstr[m++] = wordcode[n];
      wordstr[m++] = (wordcode[n] >> 8);
    }
  wordstr[m] = '\0';
  return wordstr;
}
