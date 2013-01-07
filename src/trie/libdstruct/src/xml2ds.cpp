#include "base.h"
#include "dsioformat.h"
#include "dstruct.h"
#include "baseio.h"

using namespace dstruct;

void usage()
{
  cout << "�÷�: xml2ds xml��ʽ�����ļ� ds��ʽ�����ļ�" << endl;
  cout << "˵��:" << endl;
  cout << "\t��xml��ʽ�����ļ�ת��Ϊds��ʽ�����ļ�" << endl;
}

int main(int argc, char** argv)
{
  cDStruct dstruct;
  cDSIOTextFormat textFormat;
  cDSIOXmlFormat xmlFormat;
  string dscfgfile;
  string xmlcfgfile;
  string dscfgStream;
  string xmlcfgStream;

  if (argc < 3)
    {
      usage();
      return 0;
    }

  xmlcfgfile = argv[1];
  dscfgfile = argv[2];
 
   // string resource
  string openxmlstr = "��xml��ʽ�ļ�[";
  openxmlstr += xmlcfgfile;
  openxmlstr += "]";

  string parsexmlstr = "����[";
  parsexmlstr += xmlcfgfile;
  parsexmlstr += "]";

  string outdsstr = "���ds��ʽ�ļ�[";
  outdsstr += dscfgfile;
  outdsstr += "]";

  cout << openxmlstr << "..." << endl;
  if (cBaseIo::readTextFile(xmlcfgfile, xmlcfgStream) == false)
    {
      cerr << openxmlstr << "ʧ��!!!" << endl;
      return 0;
    }

  cout << parsexmlstr << "..." << endl;
  xmlFormat.setDStructStream(xmlcfgStream);
  if (dstruct.inDStruct((iDStructIOFormat*)&xmlFormat) == false)
    {
      cerr << parsexmlstr << "ʧ��!!!" << endl;
      return 0;
    }
  cout << outdsstr << "..." << endl;
  dstruct.outDStruct((iDStructIOFormat*)&textFormat);
  dscfgStream = textFormat.getDStructStream();
  if (cBaseIo::writeTextFile(dscfgfile, dscfgStream) == false)
    {
      cerr << outdsstr << "ʧ��!!!" << endl;
      return 0;
    }
  cout << "ת�����!!!" << endl;
  return 0;
}
