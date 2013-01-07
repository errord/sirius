#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

inline bool loadData(const string& datafile, vector<string>& data)
{
  ifstream fs;
  string line;
  
  fs.open(datafile.c_str(), ios_base::in);
  if (fs.is_open() == false)
    return false;
  while (getline(fs, line)) {  
      if (line == "")
        continue;
      data.push_back(line);
    }
  return true;
}

inline bool loadData(const string& datafile, vector<const char*>& data)
{
  ifstream fs;
  string line;
  
  fs.open(datafile.c_str(), ios_base::in);
  if (fs.is_open() == false)
    return false;
  while (getline(fs, line)) {  
      if (line == "")
        continue;
      data.push_back(strdup(line.c_str()));
    }
  return true;
}

inline bool unLoadData(vector<const char*>& data)
{
  vector<const char*>::iterator iter;
  
  for (iter = data.begin(); iter != data.end(); iter++) {
    free((void*)(*iter));
  }
}

inline string formatSize(size_t size)
{
  size_t i = 0;
  stringstream s;
  
  i = size / 1000000000;
  if (i > 0)
    s << i << "G ";
  i = size / 1000000;
  if (i > 0)
    s << i%1000 << "M ";
  i = size / 1000;
  if (i > 0)
    s << i%1000 << "K ";
  s << size%1000 << "Byte";
  return s.str();
}

inline string formatTime(size_t size)
{
  size_t h;
  size_t m;
  size_t s;
  stringstream ss;
  
  h = size/(1000 * 60 * 60);
  if (h > 0)
    ss << h << "h";
  m = size/(1000 * 60);
  if (m > 0)
    ss << m%60 << "m";
  s = size/1000;
  if (s > 0)
    ss << s%60 << "s";
  ss << size%1000 << "ms";
  return ss.str();
}
