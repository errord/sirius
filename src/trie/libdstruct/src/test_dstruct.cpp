#include "base.h"
#include "dsioformat.h"
#include "dstruct.h"
#include "easycheck.h"

using namespace dstruct;

int main()
{
  cDStruct dstruct;
  cDStruct inDstruct;
  cDSIOTextFormat dsTextFormat;
  cDSIOTextFormat inDsTextFormat;
  cDSIOXmlFormat xmlFormat;
  string v;
  echeck_start("test dstruct");
  dstruct.set("t.tt", "ttt");
  v = dstruct.get("t.tt");
  ECHECK("t.tt == ttt", v == "ttt");
  dstruct.set("t.t[5]", "555");
  v = dstruct.get("t.t[5]");
  ECHECK("t.t[5] == 555", v == "555");
  dstruct.set("block.item[1]", "1000");
  dstruct.set("block.item", "item");
  v = dstruct.get("block.item[1]");
  ECHECK("block.item[1] == 1000", v == "1000");
  v = dstruct.get("block.item");
  ECHECK("block.item == item", v == "item");
  dstruct.set("bi[10][5]", "105");
  dstruct.set("bi[10].k.kk[5].kkk", "kkkkk");
  v = dstruct.get("bi[10][5]");
  ECHECK("bi[10][5] == 105", v == "105");
  v = dstruct.get("bi[10].k.kk[5].kkk");
  ECHECK("bi[10].k.kk[5].kkk == kkkkk", v == "kkkkk");
  dstruct.set("a[10][0]", "100");
  dstruct.set("a[10][1]", "101");
  dstruct.set("a[10][2]", "102");
  v = dstruct.get("a[10][0]");
  ECHECK("a[10][0] == 100", v == "100");
  v = dstruct.get("a[10][1] == 101");
  ECHECK("a[10][1] == 101", v == "101");
  v = dstruct.get("a[10][2]");
  ECHECK("a[10][2] == 102", v == "102");
  v = dstruct.get("a[9][2]");
  ECHECK("a[9][2] == \"\"", v == "");
  dstruct.set("[5].aik[2]", "aik2");
  dstruct.set("[6].aik[3].i", "aik3.i");
  v = dstruct.get("[5].aik[2]");
  ECHECK("[5].aik[2] == aik2", v == "aik2");
  v = dstruct.get("[6].aik[3].i == aik3.i");
  ECHECK("[6].aik[3].i == aik3.i", v == "aik3.i");
  dstruct.set("中文.关键字", "很好");
  v = dstruct.get("中文.关键字");
  ECHECK("中文.关键字 == 很好", v == "很好");
  dstruct.set("k.中文.数组[5][3].item[3].关键字.item[2].中国.项", "123");
  v = dstruct.get("k.中文.数组[5][3].item[3].关键字.item[2].中国.项");
  ECHECK("k.中文.数组[5][3].item[3].关键字.item[2].中国.项 == 123", v == "123");
  dstruct.outDStruct((iDStructIOFormat*)&dsTextFormat);
  string streame = dsTextFormat.getDStructStream();
  // text
  inDsTextFormat.setDStructStream(streame);
  inDstruct.inDStruct((iDStructIOFormat*)&inDsTextFormat);
  string empty = "";
  inDsTextFormat.setDStructStream(empty);
  inDstruct.outDStruct((iDStructIOFormat*)&inDsTextFormat);
  string streame2 = inDsTextFormat.getDStructStream();
  ECHECK("outDStruct == inDStruct", streame == streame2);
  // xml
  dstruct.outDStruct((iDStructIOFormat*)&xmlFormat);
  string xml = xmlFormat.getDStructStream();
  cDStruct xmlDStruct;
  xmlFormat.reset();
  xmlFormat.setDStructStream(xml);
  xmlDStruct.inDStruct((iDStructIOFormat*)&xmlFormat);
  xmlFormat.reset();
  xmlDStruct.outDStruct((iDStructIOFormat*)&xmlFormat);
  streame = xmlFormat.getDStructStream();
  ECHECK("xml == streame", xml == streame);

  // append
  cDStruct ds1;
  cDStruct ds2;
  cDStruct* ds3;
  
  ds1.set("a.b", "1");
  ds1.set("b.a[5]", "10");
  v = ds1.get("a.b");
  ECHECK("a.b == 1", v == "1");
  v = ds1.get("b.a[5]");
  ECHECK("b.a[5] == 10", v == "10");
  v = ds1.get("a.c");
  ECHECK("no a.c", v == "");
  v = ds1.get("b.b[5]");
  ECHECK("no b.b[5]", v == "");
  v = ds1.get("b.a[5].c[5]");
  ECHECK("no b.a[5].c[5]", v == "");

  
  ds2.set("a.c", "2");
  ds2.set("b.b[5]", "20");
  ds2.set("b.a[5].c[5]", "30");
  v = ds2.get("a.c");
  ECHECK("a.c == 2", v == "2");
  v = ds2.get("b.b[5]");
  ECHECK("b.b[5] == 20", v == "20");
  v = ds2.get("b.a[5].c[5]");
  ECHECK("b.a[5].c[5] == 30", v == "30");
  v = ds2.get("a.b");
  ECHECK("no a.b", v == "");
  v = ds2.get("b.a[5]");
  ECHECK("no b.a[5]", v == "");


  ds3 = ds1 + ds2;
  v = ds3->get("a.b");
  ECHECK("a.b == 1", v == "1");
  v = ds3->get("a.c");
  ECHECK("a.c == 2", v == "2");  
  v = ds3->get("b.a[5]");
  ECHECK("b.a[5] == 10", v == "10");
  v = ds3->get("b.b[5]");
  ECHECK("b.b[5] == 20", v == "20");
  v = ds3->get("b.a[5].c[5]");
  ECHECK("b.a[5].c[5] == 30", v == "30");

  /*
  string stream;
  cDSIOTextFormat ds1_textformat;
  
  ds1.outDStruct((iDStructIOFormat*)&ds1_textformat);
  stream = ds1_textformat.getDStructStream();
  cout << stream << endl;
  */

  ds1.addend(ds2);
  v = ds1.get("a.b");
  ECHECK("a.b == 1", v == "1");
  v = ds1.get("b.a[5]");
  ECHECK("b.a[5] == 10", v == "10");
  v = ds1.get("a.c");
  ECHECK("a.c == 2", v == "2");
  v = ds1.get("b.b[5]");
  ECHECK("b.b[5] == 20", v == "20");
  v = ds1.get("b.a[5].c[5]");
  ECHECK("b.a[5].c[5] == 30", v == "30");
  
  /*
  ds1_textformat.reset();
  ds1.outDStruct((iDStructIOFormat*)&ds1_textformat);
  stream = ds1_textformat.getDStructStream();
  cout << stream << endl;
  */

  /*
  string stream;
  cDSIOTextFormat ds3_textformat;
  
  ds3->outDStruct((iDStructIOFormat*)&ds3_textformat);
  stream = ds3_textformat.getDStructStream();
  cout << stream << endl;
  */
  echeck_end();
  return 0;
}
