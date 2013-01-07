#include <stdlib.h>
#include <string.h>
#include <string16.h>
#include <utf16_to_gb2312.h>
#include <siconv.h>
#include <errno.h>
#include <assert.h>

char *utf16_to_gb2312(string16 *utf16, char *gb2312, size_t gbsize)
{
	iconv_t cnv;
	char *in, *out;
	size_t isize, osize;
	size_t r;

	cnv = iconv_open("gb2312", "utf-16");
	assert(cnv != (iconv_t)-1);
	in = (char*)s16_getstr(utf16);
	out = gb2312;
	osize = gbsize;
	do {
		isize = 2;
		r = iconv(cnv, &in, &isize, &out, &osize);
	} while(*in != '\0' && r != (size_t)-1 && osize > 0);
	if (osize <= 0)
		gb2312[gbsize-1] = '\0';
	iconv_close(cnv);
    return gb2312;
}

char *u16_g2312(string16 *utf16)
{
	char *gb2312;
	size_t size = s16_len(utf16);
	size <<= 1;
	gb2312 = (char*)malloc(size);
	memset(gb2312, 0, size);
	return utf16_to_gb2312(utf16, gb2312, size);
}

