/* 
* File:   hashtable.c 哈希表的实现
* Author: netpet
* Flower net server
* 关于本程序：为了避免堆栈内存的释放不可区分导致的错误，这里要求所有name和key都必须是堆内存
*           该hash表的hash函数是java的hash函数，所以增长因子也是hash的0.75
* 本程序是为一体化web server产品专用设计，具有部分代码为具体产品优化而不代表普遍通用性的特性
* 程序在linux 2.46下调试通过，编辑工具netbeans 6.1 for c
* 联系方式：Email:netpetboy@163.com QQ:51977431
* Created on 2008年6月3日, 下午4:14
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memstate.h"
#include "hashtable.h"

unsigned int GethashValue(char *key)
{
    unsigned int hash;
    unsigned char *p;
    for(hash=0, p = (unsigned char *)key; *p ; p++)
        hash = 31 * hash + *p;
    hash=hash & 0x7FFFFFFF;
    return hash;
}

/*
*功能：指定大小的新哈希表
*参数：length：用给定的长度建立一个hashtable表
*返回：hash表
*/
HashTable * HashTableNew(int length)
{
    HashTable *hs=(HashTable *)bm_calloc(1,length*(sizeof(HashNode))+sizeof(HashTable));
    hs->size=length;
    hs->current=0;
    return hs;
}
/*
*功能：固定初始大小的哈希表
*参数：无
*返回：返回系统默认容积的hash表
*/
HashTable * NewHashTable(void)
{
    HashTable *hs=(HashTable *)bm_calloc(1,HashDefaultlength*(sizeof(HashNode))+sizeof(HashTable));
    hs->size=HashDefaultlength;
    hs->current=0;
    return hs;
}
/*
*功能：取得给定key的值
*参数： T：hash表指针 key：名称字符串
*返回：如果不存在就返回空
*/
void * HashGet(HashTable *T,char * key)
{
    unsigned int hash=GethashValue(key);
    int index=hash%T->size;
    HashNode *node=T->items[index].pt;
    while(node) {
        if(!strcmp(key, node->key)) 
            return node->value;
        node=node->next;
    }
    return NULL;
}
/*
*功能：设置一个项，不确定该项是否已经存在，如果存在就将它覆盖
*参数： T：hash表指针地址 key：名称 value：值
*返回：void
*/
void HashSet(HashTable **To,char * key,void *value)
{
    HashTable   * T=*To;
    if((T->size*75)<(T->current*100))/**大于边界0.75就扩展*/ {
        HashExpend(To);
        T=*To;
    }
    
    unsigned int hash=GethashValue(key);
    int index=hash%T->size;
    HashNode *node=T->items[index].pt;
    if(!node)
        T->items[index].pt=&T->items[T->current];
    else {
          
        while(node){
            if(!strcmp(key, node->key)) {
                node->value=value;
                return;
            }
            if(node->next)
                node=node->next;
            else
                break;
        }
        node->next=&T->items[T->current];
        node=node->next;
        node->next=NULL;
        
    }
    T->items[T->current].key=key;
    T->items[T->current].value=value;
    T->current++;
}
/*
*功能：新增一个项，可能会导致重复，但速度较快
*参数： T：hash表指针地址 key：名称 value：值
*返回：void
*/
void HashAdd(HashTable **To,char * key,void *value)
{
    HashTable   * T=*To;
    if((T->size*75)<(T->current*100))/**大于边界0.75就扩展*/ {
        HashExpend(To);
        T=*To;
    }
    unsigned int hash=GethashValue(key);
    int index=hash%T->size;
    HashNode *node;
    T->items[T->current].key=key;
    T->items[T->current].value=value;
    if(T->items[index].pt)
    {
        node=T->items[index].pt;
        while(node->next)
            node=node->next;
        node->next=&T->items[T->current];
        node=node->next;
        node->next=NULL;
    }
    else
        T->items[index].pt=&T->items[T->current];
    T->current++;
}
/*
*功能：移出指定项
*参数：T：hash表指针 key：要移出的名称
*返回：void
*/
void HashRemove(HashTable *T, char * key) {
    unsigned int hash=GethashValue(key);
    int index=hash%T->size;
    HashNode *node=T->items[index].pt, *node1;
    node1=node;
    while(node) {
        if(!strcmp(key, node->key)) {
            node->key=NULL;
            node->value=NULL;
            if(node==T->items[index].pt)
                T->items[index].pt=NULL;
            else
                node1->next=node->next;
            return;
        }
        node1=node;
        node=node->next;
    }
}
/*
*功能：是否包含指定项
*参数：T：hash表指针 key：名称
*返回：void
*/
int HashContainKey(HashTable *T,char * key)
{
   unsigned int hash=GethashValue(key);
    int index=hash%T->size;
    HashNode *node=T->items[index].pt;
    while(node) {
        if(!strcmp(key, node->key)) 
            return 1;
        node=node->next;
    }
    return 0;
}
/**拷贝两个hash表*/
void HashCopy(HashTable **Tn,HashTable *To)
{
    unsigned int i;
    HashTable *T=*Tn;
    HashNode * node=T->items;
    HashNode * nodeT=To->items;
    for(i=0;i<To->size;i++)
    {
        if(nodeT[i].key)
        {
        HashAdd(Tn,nodeT[i].key,nodeT[i].value);
        }
    }
}
/*
*功能：扩展现有的表
*参数：T；hash表指针地址
*返回：hash表
*/
HashTable * HashExpend(HashTable ** To)
{
    HashTable *T=*To;
    unsigned int length =(T->current) * 2 + 1;
    HashTable *hs=(HashTable *)bm_calloc(1, length*(sizeof(HashNode))+sizeof(HashTable));
    hs->size=length;
    hs->current=0;
    HashCopy(&hs, T);
    
    bm_free(*To);
    *To=hs;
    return hs;
}
/*
*功能：打印hash表
*参数：T：hash表指针
*返回：void
*/
void PrintHash(HashTable *T)
{
    HashNode *node=T->items,*node1;
    int i;
    for(i=0;i<T->size;i++) {
            //if(node[i].key)
            printf("当前引起的循环：%d:________________________\n",i);
                printf("%d本项：Key:%sPT:%p,Next:%p,%p\n",i, node[i].key,node[i].value,node[i].pt,node[i].next,node[i]);
            node1=node[i].pt;
            while(node1)
            {
             printf("%d含有项：Key:%s \tPT:%p,\tNext:%p,%p\n",i, node1->key,node1->value,node1->pt,node1->next,node1);
             node1=node1->next;
            }
        }
}
/*
*功能：释放一个hash表
*参数：T：hash表指针
*返回：void
*/
void HashFree(HashTable *T)
{
    bm_free(T);

}

struct _hash
{
  HashTable *hash;
};

size_t ahash_create_hashtable(unsigned int hashtable_size)
{
  _hash *hash;
  hash = (_hash*)bm_malloc(sizeof(_hash));
  hash->hash = HashTableNew(hashtable_size);
  return (size_t)hash;
}

void ahash_destroy_hashtable(size_t ht)
{
  _hash *hash;
  
  hash = (_hash*)ht;
  HashFree(hash->hash);
  bm_free(hash);
}

void ahash_ht_clear(size_t ht)
{
  _hash *hash;
  
  hash = (_hash*)ht;
  int size = hash->hash->size;
  HashFree(hash->hash);
  hash->hash = HashTableNew(size);
}

void ahash_ht_set(size_t ht, const char *key, size_t value)
{
  _hash *hash;
  
  hash = (_hash*)ht;
  HashSet(&hash->hash, (char*)key, (void*)value);
}

size_t ahash_ht_get(size_t ht, const char *key)
{
  _hash *hash;
  
  hash = (_hash*)ht;
  return (size_t)HashGet(hash->hash, (char*)key);
}

