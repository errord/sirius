#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string16.h>
#include <gb2312_to_utf16.h>
#include <utf16_to_gb2312.h>

int main()
{
	string16 s16;
	size_t i, n;
	n = 1000;
	s16_resource_init(&s16);
	for (i = 0; i < n; ++i) {
		gb2312_to_utf16("111中国人民111", &s16);
	}
	s16_resource_clear(&s16);
	return 0;
}


