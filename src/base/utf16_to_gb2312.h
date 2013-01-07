#ifndef _UTF16_TO_GB2312_H_
#define _UTF16_TO_GB2312_H_

/* conv utf16 to gb2312, max copy data size is gbsize */
char *utf16_to_gb2312(string16 *utf16, char *gb2312, size_t gbsize);

/* return new char*, need free */
char *u16_g2312(string16 *utf16);

#endif /* _UTF16_TO_GB2312_H_ */

