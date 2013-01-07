#include "util.h"
#include "memstate.h"
#include "../../libcomm/libtime.h"
#include "../../libhash/hashfun.h"
#include "../../libhash/linehash/linehash.h"
#include "../../libhash/srilmHash/srilmHash.h"
#include "../../libhash/stlhash/stlhash.h"
#include "../../libhash/fastdfshash/fastdfshash.h"
#include "../../libhash/mxhash/mxhash.h"
#include "./hash/hashtable.h"
//#include "./hash/ishashtable.h"

/*
 *
 * x:数据量 y:插入时间
 * x:数据量 y:检索时间
 * x:数据量 y:内存使用量
 *
*/

template <class HASH, class KEY, class VALUE>
struct hash_op
{
  const char* hashname;
  typedef HASH (*Create)(unsigned int hashtable_size);
  typedef void (*Destroy)(HASH t);
  typedef void (*Clear)(HASH t);
  typedef void (*Set)(HASH t, KEY key, VALUE value);
  typedef VALUE (*Get)(HASH t, KEY key);
  Create create;
  Destroy destroy;
  Clear clear;
  Set set;
  Get get;
  HASH hash;
};

template <class HASH, class KEY, class VALUE>
void benchmark_data(int datasize, vector<KEY>& data, hash_op<HASH,KEY,VALUE>& hashop)
{
  HASH hash;
  time_info_s time;
  size_t memsize = 0;
  size_t alloccount = 0;
  int insert_time = 0;
  int retrieval_time = 0;
  int invertid_retrieval_time = 0;
  int i;

  bm_resetmemsize();
  hashop.clear(hashop.hash);

  cout << "[" << hashop.hashname << "]" << " dataSize: " << datasize << endl;
  cout << " start insert.." << endl;
  /* insert */
  timestart(&time);
  for (i = 0; i < datasize; i++) {
    hashop.set(hashop.hash, data[i], 1);
  }
  insert_time = timeend(&time);
  reset_timeinfo(&time);
  
  /* memstate */
  memsize = bm_getmemsize();
  alloccount = bm_getalloccount();

  cout << " start retrieval.." << endl;
  /* retrieval */
  timestart(&time);
  for (i = 0; i < datasize; i++) {
    hashop.get(hashop.hash, data[i]);
  }
  retrieval_time = timeend(&time);
  reset_timeinfo(&time);

  /* invertid retrieval */
  timestart(&time);
  for (i = datasize-1; i >= 0; i--) {
    hashop.get(hashop.hash, data[i]);
  }
  invertid_retrieval_time = timeend(&time);
  reset_timeinfo(&time);

  
  cout << " insertTime: " << insert_time << "(" << formatTime(insert_time) << ")" <<
    " retrievalTime: " << retrieval_time << "(" << formatTime(retrieval_time) << ")" <<
    " invertidRetrievalTime: " << retrieval_time << "(" << formatTime(invertid_retrieval_time) << ")" <<
    " memSize: " << memsize << "(" << formatSize(memsize) << ")" << 
    " allocCount: " << alloccount << endl;
  bm_resetmemsize();
}

template <class HASH, class KEY, class VALUE>
void benchmark(vector<KEY> data, hash_op<HASH,KEY,VALUE>& hashop)
{
  int datasize;
  int _10;
  int _50;
  int _100;
  
  cout << "-------------------------[" << hashop.hashname << "]-------------------------" << endl;
  datasize = data.size();
  _10 = datasize / 10;
  _50 = datasize / 2;
  _100 = datasize;

  benchmark_data(_10, data, hashop);
  benchmark_data(_50, data, hashop);
  benchmark_data(_100, data, hashop);
}

int main()
{
  string datafile = "./hashfundata";
  vector<const char*> data;
  unsigned int initsize = 100;
  
  /* load data dict */
  if (loadData(datafile, data) == false) {
    cerr << "open " << datafile << " error!!!" << endl;
    return 0;
  }

  /* line hash */
  hash_op<hashtable*, const char*, unsigned int> linehash_op = {
    hashname:"linkhash",
    create:create_hashtable,
    destroy:destroy_hashtable,
    clear:ht_clear,
    set:ht_set,
    get:ht_get
  };

  linehash_op.hash = linehash_op.create(initsize);
  benchmark(data, linehash_op);
  linehash_op.destroy(linehash_op.hash);


  /* srilmHash */
  hash_op<size_t, const char*, unsigned int> srilmhash_op = {
    hashname:"srilmhash",
    create:sri_create_hashtable,
    destroy:sri_destroy_hashtable,
    clear:sri_ht_clear,
    set:sri_ht_set,
    get:sri_ht_get
  };
  
  srilmhash_op.hash = srilmhash_op.create(initsize);
  benchmark(data, srilmhash_op);
  srilmhash_op.destroy(srilmhash_op.hash);

  /* stlhash */
  hash_op<size_t, const char*, unsigned int> stlhash_op = {
    hashname:"stlhash",
    create:stl_create_hashtable,
    destroy:stl_destroy_hashtable,
    clear:stl_ht_clear,
    set:stl_ht_set,
    get:stl_ht_get
  };
  stlhash_op.hash = stlhash_op.create(initsize);
  benchmark(data, stlhash_op);
  stlhash_op.destroy(stlhash_op.hash);

  /* fastdfshash */
  hash_op<size_t, const char*, size_t> fdhash_op = {
    hashname:"fastdfshash",
    create:fdhash_create_hashtable,
    destroy:fdhash_destroy_hashtable,
    clear:fdhash_ht_clear,
    set:fdhash_ht_set,
    get:fdhash_ht_get
  };
  fdhash_op.hash = fdhash_op.create(initsize);
  benchmark(data, fdhash_op);
  fdhash_op.destroy(fdhash_op.hash);

  /* mozilla xmlparse hash */
  hash_op<size_t, const char*, unsigned int> mxhash_op = {
    hashname:"mozilla xmlparse hash",
    create:mx_create_hashtable,
    destroy:mx_destroy_hashtable,
    clear:mx_ht_clear,
    set:mx_ht_set,
    get:mx_ht_get
  };
  mxhash_op.hash = mxhash_op.create(initsize);
  benchmark(data, mxhash_op);
  mxhash_op.destroy(mxhash_op.hash);

  /* hash_a */
  hash_op<size_t, const char*, size_t> hash_a_op = {
    hashname:"hash_a",
    create:ahash_create_hashtable,
    destroy:ahash_destroy_hashtable,
    clear:ahash_ht_clear,
    set:ahash_ht_set,
    get:ahash_ht_get    
  };
  hash_a_op.hash = hash_a_op.create(initsize);
  benchmark(data, hash_a_op);
  hash_a_op.destroy(hash_a_op.hash);

  /* un load dict */
  unLoadData(data);
  return 0;
}
