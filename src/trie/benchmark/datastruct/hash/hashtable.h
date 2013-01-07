/* 
 * File:   hashtable.h 哈希表的定义
 * Author: netpet
 * Flower net server
 * 关于本程序：为了避免堆栈内存的释放不可区分导致的错误，这里要求所有name和key都必须是堆内存
 *           该hash表的hash函数是java的hash函数，所以增长因子也是hash的0.75
 * 本程序是为一体化web server产品专用设计，具有部分代码为具体产品优化而不代表普遍通用性的特性
 * 程序在linux 2.46下调试通过，编辑工具netbeans 6.1 for c
 * 联系方式：Email:netpetboy@163.com QQ:51977431
 * Created on 2008年6月3日, 下午4:14
 */

#ifndef _HASHTABLE_H
#define    _HASHTABLE_H


/* benchmark */
size_t ahash_create_hashtable(unsigned int hashtable_size);
void ahash_destroy_hashtable(size_t ht);
void ahash_ht_clear(size_t ht);
void ahash_ht_set(size_t ht, const char *key, size_t value);
size_t ahash_ht_get(size_t ht, const char *key);
/* benchmark */

#ifdef    __cplusplus
extern "C" {
#endif

#define NIL -1 //空结点标记依赖于关键字类型，本节假定关键字均为非负整数
#define HashDefaultlength 11 //表长度依赖于应用，但一般应根据。确定m为一素数
#include <stdlib.h>
  typedef struct _HashNode HashNode;
  typedef struct _hashTable HashTable;
  struct _HashNode{ //散列表结点类型
    char * key;
    void * value; //此类依赖于应用
    HashNode *next;//第一个表的连表指针
    HashNode *pt;//单独的第二个hashindex项
  };

  struct _hashTable
  {
    unsigned int size;
    unsigned int current;
    HashNode items[1];
  };
  /*
   *功能：Hash函数
   *参数：str：要转换的字符串
   *返回：经过转换的无符号的int值结果
   */
  extern unsigned int GethashValue(char *key);
  /*
   *功能：取得给定key的值
   *参数： T：hash表指针 key：名称字符串
   *返回：void
   */
  extern void * HashGet(HashTable *T,char * key);//HashSearch
  /*
   *功能：设置一个项，不确定该项是否已经存在，如果存在就将它覆盖
   *参数： T：hash表指针地址 key：名称 value：值
   *返回：void
   */
  extern void HashSet(HashTable **To,char * key,void *value);
  /*
   *功能：增加一个确信是新的项，速度比较快，不做检查，可能会导致重复
   *参数： T：hash表指针地址 key：名称 value：值
   *返回：void
   */
  extern void HashAdd(HashTable **To,char * key,void *value);
  /*
   *功能：移出指定项
   *参数：T：hash表指针 key：要移出的名称
   *返回：void
   */
  void HashRemove(HashTable *T,char * key);
  /*
   *功能：是否包含指定项
   *参数：T：hash表指针 key：名称
   *返回：void
   */
  int HashContainKey(HashTable *T,char * key);
  /*
   *功能：指定大小的新哈希表
   *参数：length：用给定的长度建立一个hashtable表
   *返回：hash表
   */
  HashTable * HashTableNew(int length);
  /*
   *功能：固定初始大小的哈希表
   *参数：无
   *返回：返回系统默认容积的hash表
   */
  HashTable * NewHashTable(void);
  /*
   *功能：扩展现有的表
   *参数：T；hash表指针地址
   *返回：hash表
   */
  HashTable * HashExpend(HashTable ** T);
  /*
   *功能：打印hash表
   *参数：T：hash表指针
   *返回：void
   */
  void PrintHash(HashTable *T);
  /*
   *功能：释放一个hash表
   *参数：T：hash表指针
   *返回：void
   */
  void HashFree(HashTable *T);

#ifdef    __cplusplus
}
#endif

#endif    /* _HASHTABLE_H */
