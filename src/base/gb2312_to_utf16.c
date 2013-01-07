#include <stdlib.h>
#include <string.h>
#include <string16.h>
#include <gb2312_to_utf16.h>
#include <siconv.h>
#include <errno.h>

static _16b getutf16BOM(iconv_t cnv)
{
	char t[1] = {'a'};
	char bom[10];
	size_t isize = 1, osize = 10;
	char *in = t;
	char *out = bom;
	iconv(cnv, &in, &isize, &out, &osize);
	return *((_16b*)bom);
}

void gb2312_to_utf16(char *gb2312, string16 *utf16)
{
	iconv_t cnv;
	size_t isize, osize, ori;
	char *in, *out;
	
	cnv = iconv_open("utf-16", "gb2312");
	getutf16BOM(cnv);
	/* scan original string */
	isize = strlen(gb2312);
	/* use x2 memory buffer */
	ori = osize = isize << 1;
	s16_resource_clear(utf16);
	s16_realloc(utf16, osize);
	in = gb2312;
	out = (char*)s16_getstr(utf16);
	/* only one call iconv to conversion all data */
	iconv(cnv, &in, &isize, &out, &osize);
	ori -= osize;
	s16_setlen(utf16, ori >> 1);
	iconv_close(cnv);
}

void gb2312_to_utf16_2(char *gb2312, string16 *utf16)
{
	iconv_t cnv;
	_16b _16bout;
	size_t isize, osize;
	char *in, *out;
	size_t r;
	
	/* no scan original string 
	 no preassign alloc memory */
	s16_resource_clear(utf16);
	cnv = iconv_open("utf-16", "gb2312");
	getutf16BOM(cnv);
	in = gb2312;
	/* continual call iconv to conversion data */
	do {
		if ((unsigned char)*in > 0x80)
			isize = 2;
		else
			isize = 1;
		out = (char*)&_16bout;
		osize = 2;
		r = iconv(cnv, &in, &isize, &out, &osize);
		if (r != -1)
			s16_append16b(utf16, _16bout);
	} while (*in != '\0' && r != -1);
	iconv_close(cnv);
}

string16 *g2312_u16(char *gb2312)
{
	string16* s16;

	s16 = s16_new(100);
	gb2312_to_utf16(gb2312, s16);
	return s16;
}

