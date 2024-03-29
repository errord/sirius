/**
* Copyright (C) 2008 Happy Fish / YuQing
*
* FastDFS may be copied only under the terms of the GNU General
* Public License V3, which may be found in the FastDFS source kit.
* Please visit the FastDFS Home Page http://www.csource.org/ for more detail.
**/

#ifndef _HASH_H_
#define _HASH_H_

#include "chain.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int (*HashFunc) (const void *key, const int key_len);

typedef struct tagHashArray
{
	ChainList *items;
	HashFunc hash_func;
	int item_count;
	unsigned int *capacity;
	double load_factor;
	bool is_malloc_capacity;
} HashArray;

typedef struct tagHashData
{
	void *key;
	int key_len;
	void *value;
	unsigned int hash_code;
} HashData;

typedef void (*HashWalkFunc)(const int index, const HashData *data, void *args);

int hash_init(HashArray *pHash, HashFunc hash_func, \
		const unsigned int capacity, const double load_factor);
void hash_destroy(HashArray *pHash);
int hash_insert(HashArray *pHash, const void *key, const int key_len, \
		void *value);
void *hash_find(HashArray *pHash, const void *key, const int key_len);
int hash_delete(HashArray *pHash, const void *key, const int key_len);
void hash_walk(HashArray *pHash, HashWalkFunc walkFunc, void *args);
int hash_best_op(HashArray *pHash, const int suggest_capacity);
void hash_stat_print(HashArray *pHash);

unsigned int RSHash(const void *key, const int key_len);
unsigned int JSHash(const void *key, const int key_len);
unsigned int PJWHash(const void *key, const int key_len);
unsigned int ELFHash(const void *key, const int key_len);
unsigned int BKDRHash(const void *key, const int key_len);
unsigned int SDBMHash(const void *key, const int key_len);
unsigned int Time33Hash(const void *key, const int key_len);
unsigned int DJBHash(const void *key, const int key_len);
unsigned int APHash(const void *key, const int key_len);
unsigned int calc_hashnr (const void* key, const int key_len);
unsigned int calc_hashnr1(const void* key, const int key_len);
unsigned int simple_hash(const void* key, const int key_len);

#ifdef __cplusplus
}
#endif

#endif

