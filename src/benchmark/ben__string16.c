#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string16.h>
#include <gb2312_to_utf16.h>
#include <utf16_to_gb2312.h>

int main()
{
	string16 s16;
	string16 *s16_2;
	size_t i , n;
	n =  500000000;
	s16_resource_init(&s16);
	s16_2 = g2312_u16("我是中国人!!!");
	//for (i = 0; i < n; ++i) {
	//	s16_append16b(&s16, 0xA1A2);
	//}
	printf("s16 length:%d bufsize:%d\n", s16_len(&s16), s16_bufsize(&s16));
	s16_resource_clear(&s16);
	n = 100000000;
	for (i = 0; i < n; ++i) {
		s16_appends16(&s16, s16_2);
	}
	printf("s16 length:%d bufsize:%d\n", s16_len(&s16), s16_bufsize(&s16));
	s16_resource_clear(&s16);
	s16_free(s16_2);
	return 0;
}

