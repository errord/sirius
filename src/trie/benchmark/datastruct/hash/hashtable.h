/* 
 * File:   hashtable.h ��ϣ��Ķ���
 * Author: netpet
 * Flower net server
 * ���ڱ�����Ϊ�˱����ջ�ڴ���ͷŲ������ֵ��µĴ�������Ҫ������name��key�������Ƕ��ڴ�
 *           ��hash���hash������java��hash������������������Ҳ��hash��0.75
 * ��������Ϊһ�廯web server��Ʒר����ƣ����в��ִ���Ϊ�����Ʒ�Ż����������ձ�ͨ���Ե�����
 * ������linux 2.46�µ���ͨ�����༭����netbeans 6.1 for c
 * ��ϵ��ʽ��Email:netpetboy@163.com QQ:51977431
 * Created on 2008��6��3��, ����4:14
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

#define NIL -1 //�ս���������ڹؼ������ͣ����ڼٶ��ؼ��־�Ϊ�Ǹ�����
#define HashDefaultlength 11 //����������Ӧ�ã���һ��Ӧ���ݡ�ȷ��mΪһ����
#include <stdlib.h>
  typedef struct _HashNode HashNode;
  typedef struct _hashTable HashTable;
  struct _HashNode{ //ɢ�б�������
    char * key;
    void * value; //����������Ӧ��
    HashNode *next;//��һ���������ָ��
    HashNode *pt;//�����ĵڶ���hashindex��
  };

  struct _hashTable
  {
    unsigned int size;
    unsigned int current;
    HashNode items[1];
  };
  /*
   *���ܣ�Hash����
   *������str��Ҫת�����ַ���
   *���أ�����ת�����޷��ŵ�intֵ���
   */
  extern unsigned int GethashValue(char *key);
  /*
   *���ܣ�ȡ�ø���key��ֵ
   *������ T��hash��ָ�� key�������ַ���
   *���أ�void
   */
  extern void * HashGet(HashTable *T,char * key);//HashSearch
  /*
   *���ܣ�����һ�����ȷ�������Ƿ��Ѿ����ڣ�������ھͽ�������
   *������ T��hash��ָ���ַ key������ value��ֵ
   *���أ�void
   */
  extern void HashSet(HashTable **To,char * key,void *value);
  /*
   *���ܣ�����һ��ȷ�����µ���ٶȱȽϿ죬������飬���ܻᵼ���ظ�
   *������ T��hash��ָ���ַ key������ value��ֵ
   *���أ�void
   */
  extern void HashAdd(HashTable **To,char * key,void *value);
  /*
   *���ܣ��Ƴ�ָ����
   *������T��hash��ָ�� key��Ҫ�Ƴ�������
   *���أ�void
   */
  void HashRemove(HashTable *T,char * key);
  /*
   *���ܣ��Ƿ����ָ����
   *������T��hash��ָ�� key������
   *���أ�void
   */
  int HashContainKey(HashTable *T,char * key);
  /*
   *���ܣ�ָ����С���¹�ϣ��
   *������length���ø����ĳ��Ƚ���һ��hashtable��
   *���أ�hash��
   */
  HashTable * HashTableNew(int length);
  /*
   *���ܣ��̶���ʼ��С�Ĺ�ϣ��
   *��������
   *���أ�����ϵͳĬ���ݻ���hash��
   */
  HashTable * NewHashTable(void);
  /*
   *���ܣ���չ���еı�
   *������T��hash��ָ���ַ
   *���أ�hash��
   */
  HashTable * HashExpend(HashTable ** T);
  /*
   *���ܣ���ӡhash��
   *������T��hash��ָ��
   *���أ�void
   */
  void PrintHash(HashTable *T);
  /*
   *���ܣ��ͷ�һ��hash��
   *������T��hash��ָ��
   *���أ�void
   */
  void HashFree(HashTable *T);

#ifdef    __cplusplus
}
#endif

#endif    /* _HASHTABLE_H */
