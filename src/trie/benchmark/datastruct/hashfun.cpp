/*
 *   维度:
 *      1. 槽个数
 *      2. 样本个数
 *      3. 计算时间
 * 
 */

#include "util.h"
#include "../../libhash/hashfun.h"
#include "../../libcomm/libtime.h"

typedef unsigned int (*hashfun)(const void* k, int key_len);

struct _info_s
{
  double clash;
  double zerorate;
  double justrate;
  double manyrate;
  double fewrate;
  int time;
};


void clash(map<int, int>& m, int s, int d, struct _info_s* info)
{
  map<int, int>::iterator sIter;
  map<int, int>::iterator eIter;
  int total = 0;
  int c;
  int zerocount = 0;
  int manycount = 0;
  int fewcount = 0;
  int justcount = 0;
  int a = d/s;
  
  a = a < 1 ? 1 : a;
  eIter = m.end();
  for (sIter = m.begin(); sIter != eIter; sIter++) {
    c = sIter->second;
    if (c < a) {
      fewcount++;
    } else if (c > a) {
      total += (c - a);
      manycount++;
    } else {
      justcount++;
    }
  }

  for (int i = 0; i < s; i++) {
    if (m[i] == 0)
      zerocount++;
  }

  info->clash = ((double)total)/((double)d);
  info->zerorate = ((double)zerocount)/((double)s);
  info->manyrate = ((double)manycount)/((double)s);
  info->fewrate = ((double)fewcount)/((double)s);
  info->justrate = ((double)justcount)/((double)s);
}

void benchmark(hashfun hf, map<int, int>& m, vector<string>& data, int s, struct _info_s* info)
{
  vector<string>::iterator sIter;
  vector<string>::iterator eIter;
  time_info_s time;
  int h;
  int c;
  
  timestart(&time);
  eIter = data.end();
  for (sIter = data.begin(); sIter != eIter; sIter++) {
    h = ( ((*hf)(sIter->c_str(), sIter->length())) % s);
    c = m[h];
    c++;
    m[h] = c;
  }
  info->time = timeend(&time);
  clash(m, s, data.size(), info);
}

void resetinfo(struct _info_s* info)
{
  info->clash = 0.0f;
  info->justrate = 0.0f;
  info->manyrate = 0.0f;
  info->fewrate = 0.0f;
  info->time = 0;
}

int main(int argc, char** argv)
{
  struct _info_s info;
  string datafile = "./hashfundata";
  vector<string> data;
  map<int, int> bm;
  double b;
  int s = 1000;
  int i;

  struct _hash_s 
  {
    char* hashname;
    hashfun hf;
  } hash_s[100] = {
    {
      hashname:"superFastHash",
      hf:superFastHash
    },
    {
      hashname:"elfhash",
      hf:elfhash
    },
    {
      hashname:"myhash",
      hf:myhash
    },
    {
      hashname:"additiveHash",
      hf:additiveHash
    },
    {
      hashname:"javahash",
      hf:javahash
    },
    {
      hashname:"RSHash",
      hf:RSHash
    },
    {
      hashname:"JSHash",
      hf:JSHash
    },
    {
      hashname:"PJWHash",
      hf:PJWHash
    },
    {
      hashname:"BKDRHash",
      hf:BKDRHash
    },
    {
      hashname:"SDBMHash",
      hf:SDBMHash
    },
    {
      hashname:"Time33Hash",
      hf:Time33Hash
    },
    {
      hashname:"DJBHash",
      hf:DJBHash
    },
    {
      hashname:"APHash",
      hf:APHash
    },
    {
      hashname:"calc_hashnr",
      hf:calc_hashnr
    },
    {
      hashname:"calc_hashnr1",
      hf:calc_hashnr1
    },
	{
      hashname:"quick_hash",
      hf:quick_hash
	},
    {
      hashname:NULL,
      hf:NULL
    }
  };
  
  if (loadData(datafile, data) == false) {
    cerr << "open " << datafile << " error!!!" << endl;
    return 0;
  }
  i = 0;
  while (hash_s[i].hashname != NULL) {
    bm.clear();
    resetinfo(&info);
    benchmark(hash_s[i].hf, bm, data, s, &info);
    cout << "[" << hash_s[i].hashname << "]" <<
      "  data:" << data.size() <<
      "  solt:" << s <<
      "  soltDataCount:" << (data.size() / s) <<
      "  zerorate:" << (info.zerorate*100) << "%" <<
      "  justrate:" << (info.justrate*100) << "%" <<
      "  manyrate:" << (info.manyrate*100) << "%" <<
      "  fewrate:" << (info.fewrate*100) << "%" << "\n" <<
      "  clash: " << (info.clash*100) << "%" << 
      "  time: " << (info.time) << endl;
    i++;
  }
  
  return 0;
}
