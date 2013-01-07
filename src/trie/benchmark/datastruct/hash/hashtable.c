/* 
* File:   hashtable.c ��ϣ���ʵ��
* Author: netpet
* Flower net server
* ���ڱ�����Ϊ�˱����ջ�ڴ���ͷŲ������ֵ��µĴ�������Ҫ������name��key�������Ƕ��ڴ�
*           ��hash���hash������java��hash������������������Ҳ��hash��0.75
* ��������Ϊһ�廯web server��Ʒר����ƣ����в��ִ���Ϊ�����Ʒ�Ż����������ձ�ͨ���Ե�����
* ������linux 2.46�µ���ͨ�����༭����netbeans 6.1 for c
* ��ϵ��ʽ��Email:netpetboy@163.com QQ:51977431
* Created on 2008��6��3��, ����4:14
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
*���ܣ�ָ����С���¹�ϣ��
*������length���ø����ĳ��Ƚ���һ��hashtable��
*���أ�hash��
*/
HashTable * HashTableNew(int length)
{
    HashTable *hs=(HashTable *)bm_calloc(1,length*(sizeof(HashNode))+sizeof(HashTable));
    hs->size=length;
    hs->current=0;
    return hs;
}
/*
*���ܣ��̶���ʼ��С�Ĺ�ϣ��
*��������
*���أ�����ϵͳĬ���ݻ���hash��
*/
HashTable * NewHashTable(void)
{
    HashTable *hs=(HashTable *)bm_calloc(1,HashDefaultlength*(sizeof(HashNode))+sizeof(HashTable));
    hs->size=HashDefaultlength;
    hs->current=0;
    return hs;
}
/*
*���ܣ�ȡ�ø���key��ֵ
*������ T��hash��ָ�� key�������ַ���
*���أ���������ھͷ��ؿ�
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
*���ܣ�����һ�����ȷ�������Ƿ��Ѿ����ڣ�������ھͽ�������
*������ T��hash��ָ���ַ key������ value��ֵ
*���أ�void
*/
void HashSet(HashTable **To,char * key,void *value)
{
    HashTable   * T=*To;
    if((T->size*75)<(T->current*100))/**���ڱ߽�0.75����չ*/ {
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
*���ܣ�����һ������ܻᵼ���ظ������ٶȽϿ�
*������ T��hash��ָ���ַ key������ value��ֵ
*���أ�void
*/
void HashAdd(HashTable **To,char * key,void *value)
{
    HashTable   * T=*To;
    if((T->size*75)<(T->current*100))/**���ڱ߽�0.75����չ*/ {
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
*���ܣ��Ƴ�ָ����
*������T��hash��ָ�� key��Ҫ�Ƴ�������
*���أ�void
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
*���ܣ��Ƿ����ָ����
*������T��hash��ָ�� key������
*���أ�void
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
/**��������hash��*/
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
*���ܣ���չ���еı�
*������T��hash��ָ���ַ
*���أ�hash��
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
*���ܣ���ӡhash��
*������T��hash��ָ��
*���أ�void
*/
void PrintHash(HashTable *T)
{
    HashNode *node=T->items,*node1;
    int i;
    for(i=0;i<T->size;i++) {
            //if(node[i].key)
            printf("��ǰ�����ѭ����%d:________________________\n",i);
                printf("%d���Key:%sPT:%p,Next:%p,%p\n",i, node[i].key,node[i].value,node[i].pt,node[i].next,node[i]);
            node1=node[i].pt;
            while(node1)
            {
             printf("%d�����Key:%s \tPT:%p,\tNext:%p,%p\n",i, node1->key,node1->value,node1->pt,node1->next,node1);
             node1=node1->next;
            }
        }
}
/*
*���ܣ��ͷ�һ��hash��
*������T��hash��ָ��
*���أ�void
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

