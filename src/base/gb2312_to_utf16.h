#ifndef _GB2312_TO_UTF16_H_
#define _GB2312_TO_UTF16_H_

void gb2312_to_utf16(char *gb2312, string16 *utf16);
void gb2312_to_utf16_2(char *gb2312, string16 *utf16);

/* return new string16, need free */
string16 *g2312_u16(char *gb2312);

#endif /* _GB2312_TO_UTF16_H_ */

