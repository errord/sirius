/*********************************************************************
* 
* File:	hashtable.hpp
* Desc:	哈希表功能相关函数和类
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
/* 哈希表功能
 * 说明：
 *			哈希表功能类和相关定义
 */
//哈希函数子
template <class Key> 
struct hash 
{ 
};

//字符串哈希函数
inline isbase::unum_t hashString(const char* pszData)
{
	isbase::unum_t nHash = 0; 
	for ( ; *pszData; ++pszData)
		nHash = 5*nHash + *pszData;
	return nHash;
}

//字符串哈希函数子
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

//整数哈希函数子
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

//哈希表模板类的前向声明
template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class EqualHandler>
class CHashTableT;

enum 
{ 
	PrimeCount = 28		//创建不同大小Bucket的质数个数
};

//bucket递增质数表
static const unsigned long PrimeList[PrimeCount] =
{
		53ul,         97ul,         193ul,       389ul,       769ul,
		1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
		49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
		1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
		50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
		1610612741ul, 3221225473ul, 4294967291ul
};

//取得下一个有效的质数
inline unsigned long getNextPrime(const isbase::unum_t a_nCount)
{
	const unsigned long* pFirst = PrimeList;
	const unsigned long* pLast = PrimeList + PrimeCount;
	const unsigned long* pPos = std::lower_bound(pFirst,pLast,a_nCount);
	return pPos==pLast? *(pLast-1) : *pPos;
}

//哈希节点结构
template <class Value>
struct SHashNode
{
	SHashNode* pNext;		//下一个节点(同一个bucket的节点间形成单向链表)
	Value Data;				//用户数据
};

/*	哈希迭代子结构
 *	说明：
 *			哈希迭代子是Forward only类型的迭代子。
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

	node* m_pCur;						//迭代子指向的节点
	hashtable* m_pHashtable;		//对哈希对象的引用

	//构造函数
	SHashIterator(node* pNode, hashtable* pHashTable) 
		: m_pCur(pNode), m_pHashtable(pHashTable) {}
	SHashIterator():m_pCur(NULL), m_pHashtable(NULL) {}

	//重载operator*操作符
	reference operator*() const 
	{ 
		return m_pCur->Data; 
	}

	pointer getp()
	{
		return &(m_pCur->Data);
	}

	//重载operator++操作符
	iterator& operator++();								//前++
	iterator operator++(int);							//后++

	//重载operator==和operator!=操作符
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

	const node* m_pCur;						//迭代子指向的节点
	const hashtable* m_pHashtable;		//对哈希对象的引用

	//构造函数
	const_SHashIterator(const node* pNode,const hashtable* pHashTable) 
		: m_pCur(pNode), m_pHashtable(pHashTable) {}
	const_SHashIterator():m_pCur(NULL), m_pHashtable(NULL) {}

	//重载operator*操作符
	reference operator*() const 
	{ 
		return m_pCur->Data; 
	}

	const pointer getp() const
	{
		return &(m_pCur->Data);
	}
	//重载operator++操作符
	const_iterator& operator++();								//前++
	const_iterator operator++(int);							//后++

	//重载operator==和operator!=操作符
	bool operator==(const const_iterator& it) const
	{ 
		return m_pCur == it.m_pCur; 
	}
	bool operator!=(const const_iterator& it) const
	{ 
		return !operator==(it);
	}
};


//CHashTableT的比较操作符前向声明
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
bool operator==(const CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>& HashTable1,
					 const CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>& HashTable2);

/*	哈希表模板类CHashTableT
 * 说明：
 *			哈希表功能的模板通用实现。
 *	模板参数：
 *			Value			：节点中存放的值或对象。
 *			Key			：		
 *			HashFunc		：哈希函数子。
 *			ExtractKey	：从Value萃取Key的函数子。
 *			EqualKey		：比较两个Key是否相等的函数子。
 *			EqualHandler：Key相同时的处理例程。
 */
template<class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler >
class CHashTableT
{
public:
	static const size_t INITBUCKETS = 100;
	//模板参数声明
	typedef Key key_type;
	typedef Value value_type;
	typedef HashFunc hasher;
	typedef ExtractKey extractor;
	typedef EqualKey comparator;
	typedef EqualHandler equal_handler;
	typedef CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler> hashtable;
	//STL规范声明
	typedef size_t            size_type;
	typedef ptrdiff_t         difference_type;
	typedef value_type*       pointer;
	typedef const value_type* const_pointer;
	typedef value_type&       reference;
	typedef const value_type& const_reference;
	//取得函数子
	hasher getHasher() const { return m_Hasher; }
	comparator getComparator() const { return m_Comparator; }
	extractor getExtractor() const { return m_Extractor; }
	equal_handler* getEqualHandler_p() { return &m_EqualHandler; }
	//友元比较操作符声明
	template <class _Value, class _Key, class _HashFunc, class _ExtractKey, 
		class _EqualKey,  class _EqualHandler>
	friend bool operator== (const CHashTableT<_Value, _Key, _HashFunc, _ExtractKey, _EqualKey, _EqualHandler>&,
		const CHashTableT<_Value, _Key, _HashFunc, _ExtractKey, _EqualKey, _EqualHandler>&);
private:
	//节点和节点内存管理器声明
	typedef SHashNode<Value> node;
private:
	//数据成员
	bool m_bUnique;						//是否允许插入重复的Key
	hasher m_Hasher;						//Hash函数子
	extractor m_Extractor;				//萃取函数子
	comparator m_Comparator;			//比较函数子
	equal_handler m_EqualHandler;		//比较处理例程
	std::vector<node*> m_Buckets;		//Bucket数组
	isbase::unum_t m_nEleCount;		//元素个数
public:
	//迭代子声明
	typedef SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler> iterator;
	typedef const_SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler> const_iterator;
	//迭代子友元声明
	friend struct
		SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>;
	friend struct
		const_SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>;
public:
	//构造函数
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
	//拷贝构造函数
	CHashTableT(const CHashTableT& HashTable)
		: m_Hasher(HashTable.m_Hasher),
		m_Comparator(HashTable.m_Comparator), m_Extractor(HashTable.m_Extractor),
		m_EqualHandler(HashTable.m_EqualHandler), m_nEleCount(0)
	{
		std::cout << "拷贝构造：this : " << (long)this <<
			", rhs : " << (long)&HashTable << endl;
		copyFrom(HashTable);
	}
	~CHashTableT()
	{
		clear();
	}
	//赋值操作符
	hashtable& operator=(const hashtable& HashTable)
	{
		if (this!=&HashTable)
		{
			std::cout << "赋值操作符：this : " << (long)this <<
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
	/*	取得更大尺寸的Bucket个数
	 *	说明：
	 *			取得比a_nCount大的质数
	 */
	isbase::unum_t getNextSize(const isbase::unum_t nCount)
	{
		return getNextPrime(nCount);
	}
	/*	初始化Bucket函数
	 * 说明：
	 *			分配合适的Bucket内存，并初始化。
	 *	参数：
	 *			a_nCount：初始化时的Key个数。
	 * 返回：
	 *			无。
	 */
	void initBuckets(const isbase::unum_t nCount)
	{
		const isbase::unum_t nBuckets = getNextSize(nCount);
		m_Buckets.reserve(nBuckets);
		m_Buckets.insert(m_Buckets.end(),nBuckets,(node*)NULL);
		m_nEleCount = 0;
	}
public:
	//返回第一个迭代子
	iterator begin()
	{ 
		for (isbase::unum_t i=0; i<m_Buckets.size(); ++i)
		{
			if (m_Buckets[i]!=NULL)		//Bucket中node指针有效
				return iterator(m_Buckets[i], this);
		}
		return end();
	}
	//返回最后一个迭代子
	iterator end() 
	{ 
		return iterator(NULL, this); 
	}
	/*	取得元素个数函数。
	 *	说明：
	 *			取得当前HashTable中的元素个数。
	 * 参数：
	 *			无。
	 *	返回：
	 *			元素个数。
	 */
	isbase::unum_t getEleCount() const 
	{
		return m_nEleCount;
	}
	//返回当前bucket个数
	isbase::unum_t getBucketCount() const
	{
		return static_cast<isbase::unum_t>(m_Buckets.size());
	}
	//取得最大支持的bucket个数
	isbase::unum_t getMaxBucket() const
	{
		return static_cast<isbase::num_t>(PrimeList[PrimeCount-1]);
	}
	//计算某个bucket中的元素个数
	isbase::unum_t eleCountInBucket(isbase::unum_t nIndex) const
	{
		isbase::unum_t nResult = 0;
		node* pCur = m_Buckets[nIndex];
		for (; pCur!=NULL; pCur=pCur->pNext)
			++nResult;
		return nResult;
	}
	/*	Key唯一插入函数。
	 *	说明：在HashTable中插入一个元素，key不允许相同。
	 *			函数可能会改变bucket大小，重建HashTable。
	 *	参数：
    *			Value：	要插入的元素。
	 * 返回：
	 *			成功返回插入位置的迭代子和true，失败返回无效迭代子和false。
	 */
	pair<iterator, bool> insert_unique(const value_type& ValData)
	{
		resize(m_nEleCount+1);
		return insert_unique_noresize(ValData);
	}
	/*	Key允许相同插入函数。
	 *	说明：在HashTable中插入一个元素，key允许相同。
	 *			函数可能会改变bucket大小，重建HashTable。
	 *	参数：
    *			Value：	要插入的元素。
	 * 返回：
	 *			成功返回插入位置的迭代子，失败返回无效迭代子。
	 */
	iterator insert_equal(const value_type& ValData)
	{
		resize(m_nEleCount+1);
		return insert_equal_noresize(ValData);
	}
	/*	Key相同特定例程插入函数。
	 *	说明：在HashTable中插入一个元素，在Key相同时调用用户例程。
	 *			函数可能会改变bucket大小，重建HashTable。
	 *	参数：
    *			Value：	要插入的元素。
	 * 返回：
	 *			成功返回插入位置的迭代子和true，失败返回无效迭代子和false。
	 *			这里返回的true/false是由用户例程决定。
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
	//不改变bucket大小的Key唯一插入函数。
	pair<iterator, bool> insert_unique_noresize(const value_type& ValData);
	//不改变bucket大小的Key允许相同插入函数。
	iterator insert_equal_noresize(const value_type& ValData);
	//不改变bucket大小的Key相同特定例程插入函数。
	pair<iterator, bool> insert_custom_noresize(const value_type& ValData);
	/*	删除node函数。
	 * 说明：以key为参数删除node。
	 *	参数：
	 *			KeyData：	某个key值。
	 *	返回：
	 *			删除的node个数。
	 */
	isbase::unum_t erase(const key_type& KeyData);
	//以iterator为参数删除某个node
	void erase(const iterator& Iter);
	//删除两个iterator之间的node
	void erase(iterator First, iterator Last);

	void erase(const const_iterator& Iter);
	void erase(const_iterator First, const_iterator Last);
	
	//确保HashTable可以容纳下ElesHint个元素
	void resize(isbase::unum_t nElesHint);
	//释放所有的node内存
	void clear();

	reference find_or_insert(const value_type& ValData);
	/*	根据key查找某个node
	 *	说明：
	 *			返回第一个找到的node。
	 *	参数：
	 *			KeyData：	要查找的Key
	 *	返回：
	 *			找到node的iterator。
	 */
	iterator find(const key_type& KeyData) 
	{
		const isbase::unum_t nIndex = getBucketbyKey(KeyData);
		node* pFirst = m_Buckets[nIndex];
		for (; pFirst && !m_Comparator(m_Extractor(pFirst->Data), KeyData); pFirst=pFirst->pNext)
		{}
		return iterator(pFirst, this);
	} 
	//取得某个key的元素个数(允许equalkey的情况)
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
	//查找某个key所在的bucket
	isbase::unum_t getBucketbyKey(const key_type& KeyData) const
	{
		return getBucketbyKey(KeyData, m_Buckets.size());
	}
	//查找某个key所在的bucket
	isbase::unum_t getBucketbyKey(const key_type& KeyData, isbase::unum_t nBuckets) const
	{
		return m_Hasher(KeyData) % nBuckets;
	}
	//查找某个value所在的bucket
	isbase::unum_t getBucketbyValue(const value_type& ValData) //const
	{
		return getBucketbyKey(m_Extractor(ValData), static_cast<isbase::unum_t>(m_Buckets.size()));
	}
	//查找某个value所在的bucket
	isbase::unum_t getBucketbyValue(const value_type& ValData, isbase::unum_t nBuckets) //const
	{
		return getBucketbyKey(m_Extractor(ValData), nBuckets);
	}
	/*	创建一个新节点函数。
	 * 说明：
	 *			分配整个节点内存，并在value空间上调用value_type的构造函数。
	 */
	node* newNode(const value_type& ValData)
	{
      node* pNode = new node;
      pNode->Data = ValData;
      pNode->pNext = NULL;
      return pNode;
	}
	/*	删除一个节点函数。
	 *	说明：
	 *			调用节点value的析构函数并释放整个节点内存。
	 */
	void deleteNode(node* pNode)
	{
      delete pNode;
	}
	/*	删除某个bucket里的节点函数。
	 *	说明：
	 *			删除某个bucket里从pFirst到pLast的所有节点。
	 *	参数：
	 *			nIndex	：bucket索引。
	 *			pFirst	：bucket里第一个要删除节点的指针。
	 *			pLast		：bucket里最后一个要删除节点的指针(NULL代表最后一个节点)。
	 *	返回：
	 *			无。
	 */
	void eraseBucket(const isbase::unum_t nIndex, node* pFirst, node* pLast);
	/*	删除某个bucket里的节点函数。
	 *	说明：
	 *			删除某个bucket里从第一个到pLast的所有节点。
	 *	参数：
	 *			nIndex	：bucket索引。
	 *			pLast		：bucket里最后一个要删除节点的指针(NULL代表最后一个节点)。
	 *	返回：
	 *			无。
	 */
	void eraseBucket(const isbase::unum_t nIndex, node* pLast);
	/*	从另一个HashTable复制节点数据。
	 */
	void copyFrom(const CHashTableT& HashTable);
};

//SHashIterator的operator++定义
template<class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>&
SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::operator++()
{
	const node* pOld = m_pCur;
	m_pCur = m_pCur->pNext;
	if (m_pCur==NULL)		//当前bucket的最后一个node
	{
		size_type nBucket = m_pHashtable->getBucketbyValue(pOld->Data);
		//找到当前bucket之后第一个node有效的bucket
		while (m_pCur==NULL && ++nBucket < m_pHashtable->getBucketCount())
			m_pCur = m_pHashtable->m_Buckets[nBucket];
	}
	return *this;
}

//SHashIterator的operator++(int)定义
template<class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::operator++(int)
{
	iterator it = *this;
	++*this;
	return it;
}

//const_SHashIterator的operator++定义
template<class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
const_SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>&
const_SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::operator++()
{
	const node* pOld = m_pCur;
	m_pCur = m_pCur->pNext;
	if (m_pCur==NULL)		//当前bucket的最后一个node
	{
		size_type nBucket = m_pHashtable->getBucketbyValue(pOld->Data);
		//找到当前bucket之后第一个node有效的bucket
		while (m_pCur==NULL && ++nBucket < m_pHashtable->getBucketCount())
			m_pCur = m_pHashtable->m_Buckets[nBucket];
	}
	return *this;
}

//const_SHashIterator的operator++(int)定义
template<class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
const_SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
const_SHashIterator<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::operator++(int)
{
	iterator it = *this;
	++*this;
	return it;
}

//CHashTableT的operator==友元操作符定义
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
		if (pCur1!=NULL || pCur2!=NULL)		//提前终止
			return false;
	}
	return true;
}  

//CHashTableT的resize定义
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
void CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>
::resize(isbase::unum_t nElesHint)
{
	const isbase::unum_t nBucketCount = static_cast<isbase::unum_t>(m_Buckets.size());
	if (nElesHint > nBucketCount)		//候捷所谓奇特的比较
	{
		const isbase::unum_t nNextSize = getNextSize(nElesHint);
		if (nNextSize > nBucketCount)		//确保不是最后一个规格(如是最后则不扩充了)
		{
			//重建bucket数组并复制数据
			vector<node*> Buckets(nNextSize, (node*)(NULL),m_Buckets.get_allocator());
			try
			{
				for (isbase::unum_t i = 0; i < nBucketCount; ++i)  //遍历老的bucket数组
				{
					node* pFirst = m_Buckets[i];							//取得当前bucket的node指针
					while (pFirst) 
					{
						//重新计算node的key在新的bucket中的位置
						isbase::unum_t nNewSize = getBucketbyValue(pFirst->Data, nNextSize);
						m_Buckets[i] = pFirst->pNext;					//把下一个node指针放入bucket
						pFirst->pNext = Buckets[nNewSize];			//next指向新bucket数组相应bucket的第一个node
						Buckets[nNewSize] = pFirst;					//当前node被串入到新bucket数组(后进的在前面,即倒了个个)
						pFirst = m_Buckets[i];							//处理下一个node     
					}
				}
				m_Buckets.swap(Buckets);								//交换bucket数组
			}
			catch (...) 
			{
			}
		}
	}
}

//CHashTableT的copyFrom定义
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
			node* pCopy = newNode(pCur->Data);	//第一个node
			m_Buckets[i] = pCopy;
			
			//之后的node
			for (node* pNext = pCur->pNext; pNext; pCur=pNext, pNext = pCur->pNext) 
			{
				pCopy->pNext = newNode(pNext->Data);		//复制node并串到自己的bucket
				pCopy = pCopy->pNext;
			}
		}
	}
	m_nEleCount = HashTable.m_nEleCount;	//元素总数
}

//CHashTableT的clear定义
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

//CHashTableT的eraseBucket定义
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

//CHashTableT的eraseBucket定义
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

//CHashTableT的find_or_insert定义
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
	m_Buckets[nIndex] = pNode;		//新的node插入在bucket头部
	++m_nEleCount;
	return pNode->Data;
}

//CHashTableT的insert_custom_noresize定义
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
		if (m_Comparator(m_Extractor(pCur->Data), m_Extractor(ValData)))	//找到相同的key
		{
			//cout << "find equal key, call equalhandler..." << endl;
			m_EqualHandler(pCur->Data,ValData);
			return pair<iterator, bool>(iterator(pCur, this), false);
		}
	}
	//没有找到这个key
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

//CHashTableT的insert_unique_noresize定义
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

//CHashTableT的insert_equal_noresize定义
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

//CHashTableT的erase定义
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

//CHashTableT的erase定义
template <class Value, class Key, class HashFunc, class ExtractKey, 
	class EqualKey, class EqualHandler>
void CHashTableT<Value,Key,HashFunc,ExtractKey,EqualKey,EqualHandler>::erase(const iterator& Iter)
{
	node* pNode = Iter->m_pCur;
	if (pNode!=NULL) 
	{
		const isbase::unum_t nIndex = getBucketbyValue(pNode->Data);
		node* pCur = m_Buckets[nIndex];
		if (pCur == pNode)	//要删除的是bucket中的第一个元素
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
				if (pNext == pNode)	//找到要删除的元素
				{
					pCur->pNext = pNext->pNext;
					deleteNode(pNext);
					--m_nEleCount;
					break;
				}
				else						//下一个元素
				{
					pCur = pNext;
					pNext = pCur->pNext;
				}
			}
		}
	}
}

//CHashTableT的erase定义
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
	else if (nFirstIndex == nLastIndex)		//处于同一个bucket内
		eraseBucket(nFirstIndex, First->m_pCur, Last->m_pCur);
	else												//跨不同的bucket
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
