#ifndef _NLCORE_WORDBASE_
#define _NLCORE_WORDBASE_

#define MEMOUT printf("mem out\n");             \
  exit(0)
#define FILEERROR(s) printf("open file [%s] error\n", s)
/* define memory allocate */
#define MALLOC malloc
#define FREE free

/**
 *
 * word process function
 *
 * 
 *
 */

struct word_s
{
  char* word;
  int wordlen;
  int freq;
  struct word_s* next;
  struct word_s* hugelink;
};

struct words_s
{
  int huge;
  int wordcount; /* use word count */
  int curindex; /* curnode index */
  int wordssize; /* real alloc words size */
  struct word_s* head;
  struct word_s* tail;
  struct word_s* hugenode; /* huge node link */
  struct word_s* curnode; /* current node */
};

struct words_s* create_result_word(int huge);

void clear_result_word(struct words_s* word);

/*
 * add result word
 */
void addresultword(struct words_s* words, char* word, int freq);

/*
 * get result word node
 */
struct word_s* get_word_node(struct words_s* words);

/*
 * reset result words
 */
void reset_result_words(struct words_s* words);


/*
 * word segment complete, curnode point result head
 */
void result_head_to_curnode(struct words_s* words);

/*
 * return result word head
 */
struct word_s* get_result_word_head(struct words_s* words);

/*
 * return result_word_count
 */
int get_result_word_count(struct words_s* words);

/*
 * word string to word code array
 * word is word string
 * wlength is word string length
 * words is word code array, return to client
 * return wordcode array size
 */
int wordcodebywordstring(const char* word, int wlength, int* words);

/*
 * word code array to word string
 * wordcode is word code array
 * wordstr is word string buff, return to client
 * wcs is wordcode array start
 * wcs is wordcode array end
 * return wordstr
 */
char* wordstringbywordcode(int* wordcode, char* wordstr, int wcs, int wce);

#endif /* _NLCORE_WORDBASE_ */
