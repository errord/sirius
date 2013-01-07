/*********************************************************************
* 
* File:	hashtable.hpp
* Desc:	��ϣ������غ�������
* Log :
* 			Create by thiefox,  2004/12/22
* 
*********************************************************************/
#ifndef _IS_HASHTABLE_H
#define _IS_HASHTABLE_H

#include <algorithm>
#include <vector>
#include <iostream>


using namespace std;

namespace isbase
{
  typedef size_t unum_t;
  typedef long num_t;
};

namespace utility
{
/* ��ϣ����
 * ˵����
 *			��ϣ���������ض���
 */
//��ϣ������
template <class Key> 
struct hash 
{ 
};

//�ַ�����ϣ����
inline isbase::unum_t hashString(const char* pszData)
{
	isbase::unum_t nHash = 0; 
	for ( ; *pszData; ++pszData)
		nHash = 5*nHash + *pszData;
	return nHash;
}

//�ַ�����ϣ������
template<> 
struct hash<char*>
{
	isbase::unum_t operator()(const char* szData) const 
	{ 
		return hashString(szData); 
	}
};

template<> 
struct hash<const char*>
{
	isbase::unum_t operator()(const char* szData) const 
	{ 
		return hashString(szData); 
	}
};

//������ϣ������
template<> 
struct hash<int> 
{
	isbase::unum_t operator()(int nData) const 
	{ 
		return nData; 
	}
};

template<> 
struct hash<unsigned int> 
{
	isbase::unum_t operator()(unsigned int nData) const 
	{ 
		return nData; 
	}
};

template<> struct hash<long> 
{
	isbase::unum_t operator()(long nData) const 
	{ 
		return nData; 
	}
};

template<> struct hash<unsigned long> 
{
	isbase::unum_t operator()(unsigned long nData) const 
	{ 
		return nData; 
	}
};

//��ϣ��ģ�����ǰ������
template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class EqualHandler>
class CHashTableT;

enum 
{ 
	PrimeCount = 28		//������ͬ��СBucket����������
};

//bucket����������
static const unsigned long PrimeList[PrimeCount] =
{
		53ul,         97ul,         193ul,       389ul,       769ul,
		1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
		49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
		1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
		50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
		1610612741ul, 3221225473ul, 4294967291ul
};

//ȡ����һ����Ч������
inline unsigned long getNextPrime(const isbase::unum_t a_nCount)
{
	const unsigned long* pFirst = PrimeList;
	const unsigned long* pLast = PrimeList + PrimeCount;
	const unsigned long* pPos = std::lower_bound(pFirst,pLast,a_nCount);
	return pPos==pLast? *(pLast-1) : *pPos;
}

//��ϣ�ڵ�ṹ
template <class Value>
struct SHashNode
{
	SHashNode* pNext;		//��һ���ڵ�(ͬһ��bucket�Ľڵ���γɵ�������)
	Value Data;				//�û�����
};

/*	��ϣ�����ӽṹ
 *	˵����
 *			��ϣ��������Forward only���͵ĵ����ӡ�
 */
template<class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler >
struct SHashIterator
{
	typedef SHashNode<Value> node;
	typedef CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler> hashtable;
	typedef SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler> iterator;

	typedef forward_iterator_tag iterator_category;
	typedef Value value_type;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef Value& reference;
	typedef Value* pointer;

	node* m_pCur;						//������ָ��Ľڵ�
	hashtable* m_pHashtable;		//�Թ�ϣ���������

	//���캯��
	SHashIterator(node* pNode, hashtable* pHashTable) 
		: m_pCur(pNode), m_pHashtable(pHashTable) {}
	SHashIterator():m_pCur(NULL), m_pHashtable(NULL) {}

	//����operator*������
	reference operator*() const 
	{ 
		return m_pCur->Data; 
	}

	pointer getp()
	{
		return &(m_pCur->Data);
	}

	//����operator++������
	iterator& operator++();								//ǰ++
	iterator operator++(int);							//��++

	//����operator==��operator!=������
	bool operator==(const iterator& it) const
	{ 
		return m_pCur == it.m_pCur; 
	}
	bool operator!=(const iterator& it) const
	{ 
		return !operator==(it);
	}
};

template<class Value, class Key, class HashFunc, class ExtractKey, 
class EqualKey, class EqualHandler >
struct const_SHashIterator
{
	typedef SHashNode<Value> node;
	typedef CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler> hashtable;
	typedef SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler> iterator;
	typedef const_SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler> const_iterator;

	typedef forward_iterator_tag iterator_category;
	typedef Value value_type;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef const value_type& reference;
	typedef const value_type* pointer;

	const node* m_pCur;						//������ָ��Ľڵ�
	const hashtable* m_pHashtable;		//�Թ�ϣ���������

	//���캯��
	const_SHashIterator(const node* pNode,const hashtable* pHashTable) 
		: m_pCur(pNode), m_pHashtable(pHashTable) {}
	const_SHashIterator():m_pCur(NULL), m_pHashtable(NULL) {}

	//����operator*������
	reference operator*() const 
	{ 
		return m_pCur->Data; 
	}

	const pointer getp() const
	{
		return &(m_pCur->Data);
	}
	//����operator++������
	const_iterator& operator++();								//ǰ++
	const_iterator operator++(int);							//��++

	//����operator==��operator!=������
	bool operator==(const const_iterator& it) const
	{ 
		return m_pCur == it.m_pCur; 
	}
	bool operator!=(const const_iterator& it) const
	{ 
		return !operator==(it);
	}
};


//CHashTableT�ıȽϲ�����ǰ������
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
bool operator==(const CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>& HashTable1,
					 const CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>& HashTable2);

/*	��ϣ��ģ����CHashTableT
 * ˵����
 *			��ϣ���ܵ�ģ��ͨ��ʵ�֡�
 *	ģ�������
 *			Value			���ڵ��д�ŵ�ֵ�����
 *			Key			��		
 *			HashFunc		����ϣ�����ӡ�
 *			ExtractKey	����Value��ȡKey�ĺ����ӡ�
 *			EqualKey		���Ƚ�����Key�Ƿ���ȵĺ����ӡ�
 *			EqualHandler��Key��ͬʱ�Ĵ������̡�
 */
template<class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler >
class CHashTableT
{
public:
	static const size_t INITBUCKETS = 100;
	//ģ���������
	typedef Key key_type;
	typedef Value value_type;
	typedef HashFunc hasher;
	typedef ExtractKey extractor;
	typedef EqualKey comparator;
	typedef EqualHandler equal_handler;
	typedef CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler> hashtable;
	//STL�淶����
	typedef size_t            size_type;
	typedef ptrdiff_t         difference_type;
	typedef value_type*       pointer;
	typedef const value_type* const_pointer;
	typedef value_type&       reference;
	typedef const value_type& const_reference;
	//ȡ�ú�����
	hasher getHasher() const { return m_Hasher; }
	comparator getComparator() const { return m_Comparator; }
	extractor getExtractor() const { return m_Extractor; }
	equal_handler* getEqualHandler_p() { return &m_EqualHandler; }
	//��Ԫ�Ƚϲ���������
	template <class _Value, class _Key, class _HashFunc, class _ExtractKey, 
		class _EqualKey,  class _EqualHandler>
	friend bool operator== (const CHashTableT<_Value, _Key, _HashFunc, _ExtractKey, _EqualKey, _EqualHandler>&,
		const CHashTableT<_Value, _Key, _HashFunc, _ExtractKey, _EqualKey, _EqualHandler>&);
private:
	//�ڵ�ͽڵ��ڴ����������
	typedef SHashNode<Value> node;
private:
	//���ݳ�Ա
	bool m_bUnique;						//�Ƿ���������ظ���Key
	hasher m_Hasher;						//Hash������
	extractor m_Extractor;				//��ȡ������
	comparator m_Comparator;			//�ȽϺ�����
	equal_handler m_EqualHandler;		//�Ƚϴ�������
	std::vector<node*> m_Buckets;		//Bucket����
	isbase::unum_t m_nEleCount;		//Ԫ�ظ���
public:
	//����������
	typedef SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler> iterator;
	typedef const_SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler> const_iterator;
	//��������Ԫ����
	friend struct
		SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>;
	friend struct
		const_SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>;
public:
	//���캯��
	CHashTableT(isbase::unum_t nCount, const HashFunc& Hasher, const ExtractKey& Extractor,
		const EqualKey& Comparator, const EqualHandler& EHandler)
		: m_Hasher(Hasher), m_Extractor(Extractor), m_Comparator(Comparator),
		m_EqualHandler(EHandler), m_nEleCount(0)
	{
		initBuckets(nCount);
	}
	CHashTableT(isbase::unum_t nCount=INITBUCKETS)
	{
		initBuckets(nCount);
	}
	//�������캯��
	CHashTableT(const CHashTableT& HashTable)
		: m_Hasher(HashTable.m_Hasher),
		m_Comparator(HashTable.m_Comparator), m_Extractor(HashTable.m_Extractor),
		m_EqualHandler(HashTable.m_EqualHandler), m_nEleCount(0)
	{
		std::cout << "�������죺this : " << (long)this <<
			", rhs : " << (long)&HashTable << endl;
		copyFrom(HashTable);
	}
	~CHashTableT()
	{
		clear();
	}
	//��ֵ������
	hashtable& operator=(const hashtable& HashTable)
	{
		if (this!=&HashTable)
		{
			std::cout << "��ֵ��������this : " << (long)this <<
				", rhs : " << (long)&HashTable << endl;
			m_Hasher = HashTable.m_Hasher;
			m_Comparator = HashTable.m_Comparator;
			m_Extractor = HashTable.m_Extractor;
			m_EqualHandler = HashTable.m_EqualHandler;
			m_nEleCount = 0;
			copyFrom(HashTable);
		}
		return *this;
	}
private:
	/*	ȡ�ø���ߴ��Bucket����
	 *	˵����
	 *			ȡ�ñ�a_nCount�������
	 */
	isbase::unum_t getNextSize(const isbase::unum_t nCount)
	{
		return getNextPrime(nCount);
	}
	/*	��ʼ��Bucket����
	 * ˵����
	 *			������ʵ�Bucket�ڴ棬����ʼ����
	 *	������
	 *			a_nCount����ʼ��ʱ��Key������
	 * ���أ�
	 *			�ޡ�
	 */
	void initBuckets(const isbase::unum_t nCount)
	{
		const isbase::unum_t nBuckets = getNextSize(nCount);
		m_Buckets.reserve(nBuckets);
		m_Buckets.insert(m_Buckets.end(),nBuckets,(node*)NULL);
		m_nEleCount = 0;
	}
public:
	//���ص�һ��������
	iterator begin()
	{ 
		for (isbase::unum_t i=0; i<m_Buckets.size(); ++i)
		{
			if (m_Buckets[i]!=NULL)		//Bucket��nodeָ����Ч
				return iterator(m_Buckets[i], this);
		}
		return end();
	}
	//�������һ��������
	iterator end() 
	{ 
		return iterator(NULL, this); 
	}
	/*	ȡ��Ԫ�ظ���������
	 *	˵����
	 *			ȡ�õ�ǰHashTable�е�Ԫ�ظ�����
	 * ������
	 *			�ޡ�
	 *	���أ�
	 *			Ԫ�ظ�����
	 */
	isbase::unum_t getEleCount() const 
	{
		return m_nEleCount;
	}
	//���ص�ǰbucket����
	isbase::unum_t getBucketCount() const
	{
		return static_cast<isbase::unum_t>(m_Buckets.size());
	}
	//ȡ�����֧�ֵ�bucket����
	isbase::unum_t getMaxBucket() const
	{
		return static_cast<isbase::num_t>(PrimeList[PrimeCount-1]);
	}
	//����ĳ��bucket�е�Ԫ�ظ���
	isbase::unum_t eleCountInBucket(isbase::unum_t nIndex) const
	{
		isbase::unum_t nResult = 0;
		node* pCur = m_Buckets[nIndex];
		for (; pCur!=NULL; pCur=pCur->pNext)
			++nResult;
		return nResult;
	}
	/*	KeyΨһ���뺯����
	 *	˵������HashTable�в���һ��Ԫ�أ�key��������ͬ��
	 *			�������ܻ�ı�bucket��С���ؽ�HashTable��
	 *	������
    *			Value��	Ҫ�����Ԫ�ء�
	 * ���أ�
	 *			�ɹ����ز���λ�õĵ����Ӻ�true��ʧ�ܷ�����Ч�����Ӻ�false��
	 */
	pair<iterator, bool> insert_unique(const value_type& ValData)
	{
		resize(m_nEleCount+1);
		return insert_unique_noresize(ValData);
	}
	/*	Key������ͬ���뺯����
	 *	˵������HashTable�в���һ��Ԫ�أ�key������ͬ��
	 *			�������ܻ�ı�bucket��С���ؽ�HashTable��
	 *	������
    *			Value��	Ҫ�����Ԫ�ء�
	 * ���أ�
	 *			�ɹ����ز���λ�õĵ����ӣ�ʧ�ܷ�����Ч�����ӡ�
	 */
	iterator insert_equal(const value_type& ValData)
	{
		resize(m_nEleCount+1);
		return insert_equal_noresize(ValData);
	}
	/*	Key��ͬ�ض����̲��뺯����
	 *	˵������HashTable�в���һ��Ԫ�أ���Key��ͬʱ�����û����̡�
	 *			�������ܻ�ı�bucket��С���ؽ�HashTable��
	 *	������
    *			Value��	Ҫ�����Ԫ�ء�
	 * ���أ�
	 *			�ɹ����ز���λ�õĵ����Ӻ�true��ʧ�ܷ�����Ч�����Ӻ�false��
	 *			���ﷵ�ص�true/false�����û����̾�����
	 */
	pair<iterator, bool> insert_custom(const value_type& ValData)
	{
		resize(m_nEleCount+1);
		return insert_custom_noresize(ValData);
	}
	pair<iterator, bool> insert(const value_type& ValData)
	{
		return insert_custom(ValData);
	}
	//���ı�bucket��С��KeyΨһ���뺯����
	pair<iterator, bool> insert_unique_noresize(const value_type& ValData);
	//���ı�bucket��С��Key������ͬ���뺯����
	iterator insert_equal_noresize(const value_type& ValData);
	//���ı�bucket��С��Key��ͬ�ض����̲��뺯����
	pair<iterator, bool> insert_custom_noresize(const value_type& ValData);
	/*	ɾ��node������
	 * ˵������keyΪ����ɾ��node��
	 *	������
	 *			KeyData��	ĳ��keyֵ��
	 *	���أ�
	 *			ɾ����node������
	 */
	isbase::unum_t erase(const key_type& KeyData);
	//��iteratorΪ����ɾ��ĳ��node
	void erase(const iterator& Iter);
	//ɾ������iterator֮���node
	void erase(iterator First, iterator Last);

	void erase(const const_iterator& Iter);
	void erase(const_iterator First, const_iterator Last);
	
	//ȷ��HashTable����������ElesHint��Ԫ��
	void resize(isbase::unum_t nElesHint);
	//�ͷ����е�node�ڴ�
	void clear();

	reference find_or_insert(const value_type& ValData);
	/*	����key����ĳ��node
	 *	˵����
	 *			���ص�һ���ҵ���node��
	 *	������
	 *			KeyData��	Ҫ���ҵ�Key
	 *	���أ�
	 *			�ҵ�node��iterator��
	 */
	iterator find(const key_type& KeyData) 
	{
		const isbase::unum_t nIndex = getBucketbyKey(KeyData);
		node* pFirst = m_Buckets[nIndex];
		for (; pFirst && !m_Comparator(m_Extractor(pFirst->Data), KeyData); pFirst=pFirst->pNext)
		{}
		return iterator(pFirst, this);
	} 
	//ȡ��ĳ��key��Ԫ�ظ���(����equalkey�����)
	isbase::unum_t getCountbyKey(const key_type& KeyData) const
	{
		const isbase::unum_t nIndex = getBucketbyKey(KeyData);
		isbase::unum_t nCount = 0;
		for (const node* pCur = m_Buckets[nIndex]; pCur; pCur = pCur->pNext)
		{
			if (m_Comparator(m_Extractor(pCur->Data), KeyData))
				++nCount;
		}
		return nCount;
	}
	//����ĳ��key���ڵ�bucket
	isbase::unum_t getBucketbyKey(const key_type& KeyData) const
	{
		return getBucketbyKey(KeyData, m_Buckets.size());
	}
	//����ĳ��key���ڵ�bucket
	isbase::unum_t getBucketbyKey(const key_type& KeyData, isbase::unum_t nBuckets) const
	{
		return m_Hasher(KeyData) % nBuckets;
	}
	//����ĳ��value���ڵ�bucket
	isbase::unum_t getBucketbyValue(const value_type& ValData) //const
	{
		return getBucketbyKey(m_Extractor(ValData), static_cast<isbase::unum_t>(m_Buckets.size()));
	}
	//����ĳ��value���ڵ�bucket
	isbase::unum_t getBucketbyValue(const value_type& ValData, isbase::unum_t nBuckets) //const
	{
		return getBucketbyKey(m_Extractor(ValData), nBuckets);
	}
	/*	����һ���½ڵ㺯����
	 * ˵����
	 *			���������ڵ��ڴ棬����value�ռ��ϵ���value_type�Ĺ��캯����
	 */
	node* newNode(const value_type& ValData)
	{
      node* pNode = new node;
      pNode->Data = ValData;
      pNode->pNext = NULL;
      return pNode;
	}
	/*	ɾ��һ���ڵ㺯����
	 *	˵����
	 *			���ýڵ�value�������������ͷ������ڵ��ڴ档
	 */
	void deleteNode(node* pNode)
	{
      delete pNode;
	}
	/*	ɾ��ĳ��bucket��Ľڵ㺯����
	 *	˵����
	 *			ɾ��ĳ��bucket���pFirst��pLast�����нڵ㡣
	 *	������
	 *			nIndex	��bucket������
	 *			pFirst	��bucket���һ��Ҫɾ���ڵ��ָ�롣
	 *			pLast		��bucket�����һ��Ҫɾ���ڵ��ָ��(NULL�������һ���ڵ�)��
	 *	���أ�
	 *			�ޡ�
	 */
	void eraseBucket(const isbase::unum_t nIndex, node* pFirst, node* pLast);
	/*	ɾ��ĳ��bucket��Ľڵ㺯����
	 *	˵����
	 *			ɾ��ĳ��bucket��ӵ�һ����pLast�����нڵ㡣
	 *	������
	 *			nIndex	��bucket������
	 *			pLast		��bucket�����һ��Ҫɾ���ڵ��ָ��(NULL�������һ���ڵ�)��
	 *	���أ�
	 *			�ޡ�
	 */
	void eraseBucket(const isbase::unum_t nIndex, node* pLast);
	/*	����һ��HashTable���ƽڵ����ݡ�
	 */
	void copyFrom(const CHashTableT& HashTable);
};

//SHashIterator��operator++����
template<class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>&
SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::operator++()
{
	const node* pOld = m_pCur;
	m_pCur = m_pCur->pNext;
	if (m_pCur==NULL)		//��ǰbucket�����һ��node
	{
		size_type nBucket = m_pHashtable->getBucketbyValue(pOld->Data);
		//�ҵ���ǰbucket֮���һ��node��Ч��bucket
		while (m_pCur==NULL && ++nBucket < m_pHashtable->getBucketCount())
			m_pCur = m_pHashtable->m_Buckets[nBucket];
	}
	return *this;
}

//SHashIterator��operator++(int)����
template<class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::operator++(int)
{
	iterator it = *this;
	++*this;
	return it;
}

//const_SHashIterator��operator++����
template<class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
const_SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>&
const_SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::operator++()
{
	const node* pOld = m_pCur;
	m_pCur = m_pCur->pNext;
	if (m_pCur==NULL)		//��ǰbucket�����һ��node
	{
		size_type nBucket = m_pHashtable->getBucketbyValue(pOld->Data);
		//�ҵ���ǰbucket֮���һ��node��Ч��bucket
		while (m_pCur==NULL && ++nBucket < m_pHashtable->getBucketCount())
			m_pCur = m_pHashtable->m_Buckets[nBucket];
	}
	return *this;
}

//const_SHashIterator��operator++(int)����
template<class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
const_SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
const_SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::operator++(int)
{
	iterator it = *this;
	++*this;
	return it;
}

//CHashTableT��operator==��Ԫ����������
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
bool operator==(const CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>& HashTable1,
					 const CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>& HashTable2)
{
	typedef typename CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::node node;
	if (HashTable1.m_Buckets.size()!=HashTable2.m_Buckets.size())
		return false;
	for (isbase::unum_t i = 0; i < HashTable1.m_Buckets.size(); ++i) 
	{
		node* pCur1 = HashTable1.m_Buckets[i];
		node* pCur2 = HashTable2.m_Buckets[i];
		for ( ; pCur1 && pCur2 && pCur1->Data==pCur2->Data;
			pCur1 = pCur1->pNext, pCur2 = pCur2->pNext)
		{}
		if (pCur1!=NULL || pCur2!=NULL)		//��ǰ��ֹ
			return false;
	}
	return true;
}  

//CHashTableT��resize����
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
void CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
::resize(isbase::unum_t nElesHint)
{
	const isbase::unum_t nBucketCount = static_cast<isbase::unum_t>(m_Buckets.size());
	if (nElesHint > nBucketCount)		//�����ν���صıȽ�
	{
		const isbase::unum_t nNextSize = getNextSize(nElesHint);
		if (nNextSize > nBucketCount)		//ȷ���������һ�����(���������������)
		{
			//�ؽ�bucket���鲢��������
			vector<node*> Buckets(nNextSize, (node*)(NULL),m_Buckets.get_allocator());
			try
			{
				for (isbase::unum_t i = 0; i < nBucketCount; ++i)  //�����ϵ�bucket����
				{
					node* pFirst = m_Buckets[i];							//ȡ�õ�ǰbucket��nodeָ��
					while (pFirst) 
					{
						//���¼���node��key���µ�bucket�е�λ��
						isbase::unum_t nNewSize = getBucketbyValue(pFirst->Data, nNextSize);
						m_Buckets[i] = pFirst->pNext;					//����һ��nodeָ�����bucket
						pFirst->pNext = Buckets[nNewSize];			//nextָ����bucket������Ӧbucket�ĵ�һ��node
						Buckets[nNewSize] = pFirst;					//��ǰnode�����뵽��bucket����(�������ǰ��,�����˸���)
						pFirst = m_Buckets[i];							//������һ��node     
					}
				}
				m_Buckets.swap(Buckets);								//����bucket����
			}
			catch (...) 
			{
			}
		}
	}
}

//CHashTableT��copyFrom����
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
void CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
::copyFrom(const CHashTableT& HashTable)
{
	m_Buckets.clear();
	m_Buckets.reserve(HashTable.m_Buckets.size());
	m_Buckets.insert(m_Buckets.end(),m_Buckets.size(),(node*)NULL);
	for (size_type i = 0; i < HashTable.m_Buckets.size(); ++i) 
	{
		const node* pCur = HashTable.m_Buckets[i];
		if (pCur!=NULL) 
		{
			node* pCopy = newNode(pCur->Data);	//��һ��node
			m_Buckets[i] = pCopy;
			
			//֮���node
			for (node* pNext = pCur->pNext; pNext; pCur=pNext, pNext = pCur->pNext) 
			{
				pCopy->pNext = newNode(pNext->Data);		//����node�������Լ���bucket
				pCopy = pCopy->pNext;
			}
		}
	}
	m_nEleCount = HashTable.m_nEleCount;	//Ԫ������
}

//CHashTableT��clear����
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
void CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::clear()
{
	if (m_nEleCount==0)
		return;

	size_t nCount = 0;
	for (size_type i = 0; i < m_Buckets.size(); ++i) 
	{
		node* pCur = m_Buckets[i];
		while (pCur!=NULL) 
		{
			node* pNext = pCur->pNext;
			value_type* pValue = &(pCur->Data);
			deleteNode(pCur);
			pCur = pNext;
		}
		m_Buckets[i] = NULL;
	}
	m_nEleCount = 0;
}

//CHashTableT��eraseBucket����
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
void CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
::eraseBucket(const isbase::unum_t nIndex, node* pFirst, node* pLast)
{
	node* pCur = m_Buckets[nIndex];
	if (pCur == pFirst)
		eraseBucket(nIndex, pLast);
	else 
	{
		node* pNext = pCur->pNext;
		for ( ; pNext != pFirst; pCur = pNext, pNext = pCur->pNext)
		{
		}
		while (pNext != pLast) 
		{
			pCur->pNext = pNext->pNext;
			deleteNode(pNext);
			pNext = pCur->pNext;
			--m_nEleCount;
		}
	}
}

//CHashTableT��eraseBucket����
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
void CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
::eraseBucket(const isbase::unum_t nIndex, node* pLast)
{
	node* pCur = m_Buckets[nIndex];
	while (pCur != pLast) 
	{
		node* pNext = pCur->pNext;
		deleteNode(pCur);
		pCur = pNext;
		m_Buckets[nIndex] = pCur;
		--m_nEleCount;
	}
}

//CHashTableT��find_or_insert����
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
typename CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::reference
CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::find_or_insert(const value_type& ValData)
{
	resize(m_nEleCount+1);

	isbase::unum_t nIndex = getBucketbyValue(ValData);
	node* pFirst = m_Buckets[nIndex];

	for (node* pCur = pFirst; pCur; pCur = pCur->pNext)
	{
		if (m_Comparator(m_Extractor(pCur->Data), m_Extractor(ValData)))
			return pCur->Data;
	}
	
	node* pNode = newNode(ValData);
	pNode->pNext = pFirst;
	m_Buckets[nIndex] = pNode;		//�µ�node������bucketͷ��
	++m_nEleCount;
	return pNode->Data;
}

//CHashTableT��insert_custom_noresize����
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
pair<typename CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::iterator, bool> 
CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
::insert_custom_noresize(const value_type& ValData)
{
	const size_type nBucket = getBucketbyValue(ValData);
	node* pFirst = m_Buckets[nBucket];

	for (node* pCur = pFirst; pCur; pCur = pCur->pNext)
	{
		if (m_Comparator(m_Extractor(pCur->Data), m_Extractor(ValData)))	//�ҵ���ͬ��key
		{
			//cout << "find equal key, call equalhandler..." << endl;
			m_EqualHandler(pCur->Data,ValData);
			return pair<iterator, bool>(iterator(pCur, this), false);
		}
	}
	//û���ҵ����key
	node* pNode = newNode(ValData);
	pNode->pNext = pFirst;
	m_Buckets[nBucket] = pNode;
	++m_nEleCount;
	//cout << "insert new key, elecount : " << m_nEleCount << endl;
	/*
	if (m_nEleCount>=615 && m_nEleCount<=617)
	{
		value_type* pValue = &(pNode->Data);

		cout << "ele count " << m_nEleCount << ", item point : " << (long)pValue 
			<< ", this : " << (long)this << endl;
	}
	*/
	return pair<iterator, bool>(iterator(pNode, this), true);
}

//CHashTableT��insert_unique_noresize����
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
pair<typename CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::iterator, bool> 
CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
::insert_unique_noresize(const value_type& ValData)
{
	const size_type nBucket = getBucketbyValue(ValData);
	node* pFirst = m_Buckets[nBucket];

	for (node* pCur = pFirst; pCur; pCur = pCur->pNext)
	{
		if (m_Comparator(m_Extractor(pCur->Data), m_Extractor(ValData)))
			return pair<iterator, bool>(iterator(pCur, this), false);
	}
	node* pNode = newNode(ValData);
	
	pNode->pNext = pFirst;
	m_Buckets[nBucket] = pNode;
	++m_nEleCount;
	return pair<iterator, bool>(iterator(pNode, this), true);
}

//CHashTableT��insert_equal_noresize����
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
typename CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::iterator 
CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
::insert_equal_noresize(const value_type& ValData)
{
	const isbase::unum_t nIndex = getBucketbyValue(ValData);
	node* pFirst = m_Buckets[nIndex];

	for (node* pCur = pFirst; pCur; pCur = pCur->pNext)
	{
		if (m_Comparator(m_Extractor(pCur->Data), m_Extractor(ValData))) 
		{
			node* pNode = newNode(ValData);
			pNode->pNext = pCur->pNext;
			pCur->pNext = pNode;
			++m_nEleCount;
			return iterator(pNode, this);
		}
	}
	
	node* pNode = newNode(ValData);
	pNode->pNext = pFirst;
	m_Buckets[nIndex] = pNode;
	++m_nEleCount;
	return iterator(pNode, this);
}

//CHashTableT��erase����
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
isbase::unum_t CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::erase(const key_type& KeyData)
{
	const isbase::unum_t nIndex = getBucketbyKey(KeyData);
	node* pFirst = m_Buckets[nIndex];

	isbase::unum_t nCount = 0;

	if (pFirst!=NULL) 
	{
		node* pCur = pFirst;
		node* pNext = pCur->pNext;
		while (pNext) 
		{
			if (m_Comparator(m_Extractor(pNext->Data), KeyData)) 
			{
				pCur->pNext = pNext->pNext;
				deleteNode(pNext);
				pNext = pCur->pNext;
				++nCount;
				--m_nEleCount;
			}
			else 
			{
				pCur = pNext;
				pNext = pCur->pNext;
			}
		}
		if (m_Comparator(m_Extractor(pFirst->Data), KeyData)) 
		{
			m_Buckets[nIndex] = pFirst->pNext;
			deleteNode(pFirst);
			++nCount;
			--m_nEleCount;
		}
	}
	return nCount;
}

//CHashTableT��erase����
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
void CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::erase(const iterator& Iter)
{
	node* pNode = Iter->m_pCur;
	if (pNode!=NULL) 
	{
		const isbase::unum_t nIndex = getBucketbyValue(pNode->Data);
		node* pCur = m_Buckets[nIndex];
		if (pCur == pNode)	//Ҫɾ������bucket�еĵ�һ��Ԫ��
		{
			m_Buckets[nIndex] = pCur->pNext;
			deleteNode(pCur);
			--m_nEleCount;
		}
		else 
		{
			node* pNext = pCur->pNext;
			while (pNext!=NULL) 
			{
				if (pNext == pNode)	//�ҵ�Ҫɾ����Ԫ��
				{
					pCur->pNext = pNext->pNext;
					deleteNode(pNext);
					--m_nEleCount;
					break;
				}
				else						//��һ��Ԫ��
				{
					pCur = pNext;
					pNext = pCur->pNext;
				}
			}
		}
	}
}

//CHashTableT��erase����
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
void CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
::erase(iterator First, iterator Last)
{
	isbase::unum_t nFirstIndex = First.m_pCur ?  
		getBucketbyValue(First.m_pCur->Data) : m_Buckets.size();
	isbase::unum_t nLastIndex = Last.m_pCur ? 
		getBucketbyValue(Last.m_pCur->Data) : m_Buckets.size();

	if (First.m_pCur = Last.m_pCur)
		return;
	else if (nFirstIndex == nLastIndex)		//����ͬһ��bucket��
		eraseBucket(nFirstIndex, First->m_pCur, Last->m_pCur);
	else												//�粻ͬ��bucket
	{
		eraseBucket(nFirstIndex, First.m_pCur, NULL);
		for (isbase::unum_t i = nFirstIndex+1; i<nLastIndex; ++i)
			eraseBucket(i, NULL);
		if (nLastIndex!=m_Buckets.size())
			eraseBucket(nLastIndex,Last->m_pCur);
	}
}

template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
void CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
::erase(const const_iterator& Iter)
{
	erase(iterator(const_cast<node*>(Iter.m_pCur),
		const_cast<hashtable*>(Iter.m_pHashtable)));
}

template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
void CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
::erase(const_iterator First, const_iterator Last)
{
	erase(iterator(const_cast<node*>(First.m_pCur),
		const_cast<hashtable*>(First.m_pHashtable)),
		iterator(const_cast<node*>(Last.m_pCur),
		const_cast<hashtable*>(Last.m_pHashtable)));
}


}	//namesapce utiltiy



struct hd
{
  const char *key;
  int value;
};

const char * bExtractKey(hd d)
{
  return d.key;
};

bool bEqualKey(const char *key, const char *key2)
{
  return strcmp(key, key2) == 0 ? true : false;
};

void bEqualHandler(hd d, hd dd)
{
  
};

/* benchmark */
inline int is_create_hashtable(unsigned int hashtable_size)
{
  int hash;

  utility::CHashTableT<struct hb, const char *, struct hash<const char*>, bExtractKey, bEqualKey, bEqualHandler> hashtable;
  
}

inline void is_destroy_hashtable(int ht)
{
  
}

inline void is_ht_clear(int ht)
{
  
}

inline void is_ht_set(int ht, const char *key, unsigned int value)
{
  
}

inline unsigned int is_ht_get(int ht, const char *key)
{
  
}

/* benchmark */



#endif
