/*
 *
 * clib -- libdatrie
 * dict datrie
 *
 */
#ifndef _CLIB_LIBDATRIE_DICTDATRIE_H_
#define _CLIB_LIBDATRIE_DICTDATRIE_H_

#define DDMEMOUT CLIBMEMOUT
#define DDMEMOUT_EXIT(ret) CLIBMEMOUT
/* define memory allocate */
#define DDMALLOC CLIBMALLOC
#define DDFREE CLIBFREE

/* Version */
#define MAKE_VERSION(major, minor) (char)(major<<4 | minor)
#define MAJOR_VERSION(ver) (ver>>4 & 0x0F)
#define MINOR_VERSION(ver) (ver & 0x0F)


/* Magic */
#define DATRIE_MAGIC (const char*)"DATRIEBINDICT"

/* Version */
#define DATRIE_VERSION_1_0 MAKE_VERSION(1,0)

/*
 * save datrie to binary dictionary
 * datrie struct
 * dictionary file name
 * return 0 save failed
 * return 1 save success
 */
int savedatrie_bindict(struct datrietree_s* datrietree, const char* dictname);

/*
 * load binary dictionary to datrie
 */
struct datrietree_s* loaddatrie_bindict(const char* dictname);

#endif /* _CLIB_LIBDATRIE_DICTDATRIE_H_ */
