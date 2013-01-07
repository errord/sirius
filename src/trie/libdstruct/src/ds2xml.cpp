#include "base.h"
#include "dsioformat.h"
#include "dstruct.h"
#include "baseio.h"

using namespace dstruct;

void usage()
{
  cout << "�÷�: ds2xml ds��ʽ�����ļ� xml��ʽ�����ļ�" << endl;
  cout << "˵��:" << endl;
  cout << "\t��ds��ʽ�����ļ�ת��Ϊxml��ʽ�����ļ�" << endl;
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

  dscfgfile = argv[1];
  xmlcfgfile = argv[2];
 
   // string resource
  string opendsstr = "��ds��ʽ�ļ�[";
  opendsstr += dscfgfile;
  opendsstr += "]";

  string parsedsstr = "����[";
  parsedsstr += dscfgfile;
  parsedsstr += "]";

  string outxmlstr = "���xml��ʽ�ļ�[";
  outxmlstr += xmlcfgfile;
  outxmlstr += "]";

  cout << opendsstr << "..." << endl;
  if (cBaseIo::readTextFile(dscfgfile, dscfgStream) == false)
    {
      cerr << opendsstr << "ʧ��!!!" << endl;
      return 0;
    }

  cout << parsedsstr << "..." << endl;
  textFormat.setDStructStream(dscfgStream);
  if (dstruct.inDStruct((iDStructIOFormat*)&textFormat) == false)
    {
      cerr << parsedsstr << "ʧ��!!!" << endl;
      return 0;
    }
  cout << outxmlstr << "..." << endl;
  dstruct.outDStruct((iDStructIOFormat*)&xmlFormat);
  xmlcfgStream = xmlFormat.getDStructStream();
  if (cBaseIo::writeTextFile(xmlcfgfile, xmlcfgStream) == false)
    {
      cerr << outxmlstr << "ʧ��!!!" << endl;
      return 0;
    }
  cout << "ת�����!!!" << endl;
  return 0;
}
