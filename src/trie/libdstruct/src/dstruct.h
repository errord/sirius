#ifndef _D_STRUCT_H_
#define _D_STRUCT_H_

namespace dstruct
{

  enum DSPathNodeType
    {
      unknownode,
      mapnode,
      arraynode
    };
  
  /* 路径节点 */
  class cDSPathNode
  {
  public:
    cDSPathNode();
    virtual ~cDSPathNode();

    void clear();
    void setType(DSPathNodeType type);
    DSPathNodeType getType();
    void setKey(const string& key);
    string& getKey();
    void setIndex(int index);
    int getIndex();
    
  private:
    DSPathNodeType mType;
    string mKey;
    int mIndex;
  };

  /* 路径解析 */
  class cDSPathParse
  {
  public:
    cDSPathParse();
    ~cDSPathParse();
    
    void setPath(const string& path);
    void reset();
    bool nextPathToken(cDSPathNode& pathNode);
    
  private:
    string mPath;
    const char* mPoint;
  };

  /* DStruct节点 */
  class cDStructNode
  {
  public:
    typedef map<string, cDStructNode*> mapPoolDef;
    typedef mapPoolDef* mapPoolPoint;
    typedef vector<cDStructNode*> arrayPoolDef;
    typedef arrayPoolDef* arrayPoolPoint;
    typedef map<string, cDStructNode*>::iterator mapPoolIter;
    typedef vector<cDStructNode*>::iterator arrayPoolIter;
  
    // construct destroy
  public:
    cDStructNode();
    virtual ~cDStructNode();
    cDStructNode(const cDStructNode& dstruct);
    cDStructNode(const cDStructNode* dstruct);
    
  public:
    mapPoolPoint copyMapPool() const;
    arrayPoolPoint copyArrayPool() const;
    void nodeClear();
    void clearMapPool();
    void clearArrayPool();
    void set(const string& path, const string& value);
    cDStructNode* getNode(const string& path);
    cDStructNode* insert(cDSPathNode& node);
    cDStructNode* find(cDSPathNode& node);
    string getValue();
    cDStructNode* getMapNode(const string& key);
    cDStructNode* getArrayNode(unsigned int index);
    void setValue(const string& value);
    void addMapNode(const string& key, cDStructNode* node);
    void addArrayNode(unsigned int index, cDStructNode* node);
    mapPoolIter mapPoolBegin();
    mapPoolIter mapPoolEnd();
    arrayPoolIter arrayPoolBegin();
    arrayPoolIter arrayPoolEnd();

  private:
    string mValue;
    mapPoolPoint mMapPool;
    arrayPoolPoint mArrayPool;
  };

  class cDStruct
  {
  public:
    cDStruct();
    cDStruct(iDStructIOFormat* inFormat);
    cDStruct(cDStructNode* structNode);
    virtual ~cDStruct();
      
    void clear();
    void set(const string& path, const string& value);
    string get(const string& path);
    cDStructNode* getRootNode();
    void outDStruct(iDStructIOFormat* outFormat);
    bool inDStruct(iDStructIOFormat* inFormat);
    void traipsePath(cDStructNode* node, iDStructIOFormat* outFormat);
    void addend(cDStruct& dstruct);
    cDStruct* operator+(cDStruct& dstruct);

  private:
    cDStructNode mDSnode;
  };
};

#endif // _D_STRUCT_H_
