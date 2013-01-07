#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <easycheck.h>
#include <string16.h>

int main()
{
	string16 s;
	string16 *small_s;
	string16 *ns;
	char buf[1024];
	char *info = "String16";
	echeck_start(info);
	s16_resource_init(&s);
	/* case */
	s16_realloc(&s, 100);
	sprintf(buf, "s16_realloc(%d) < stack max size(%d),use stack", 100, S16_STACKBUF_MAX);
	ECHECK(buf, s.str == s.stackbuf);
	/* case */
	s16_realloc(&s, 2048);
	sprintf(buf, "s16_realloc(%d) > stack max size(%d),no use stack", 2048, S16_STACKBUF_MAX);
	ECHECK(buf, s.str != s.stackbuf);
	/* case */
	s16_realloc(&s, 1023);
	sprintf(buf, "s16_realloc(%d) < stack max size(%d),use stack", 1023, S16_STACKBUF_MAX);
	ECHECK(buf, s.str == s.stackbuf);
	/* case */
	s16_resource_clear(&s);
	small_s = s16_stacksize_new(100, 10);
	ns = s16_new(10);
	printf("[debug] small_s(%d) < ns(%d)\n", sizeof(*small_s), sizeof(*ns));
	/* case */
	s16_realloc(small_s, 10);
	sprintf(buf, "small_s s16_realloc(%d) < stack max size(%d),use stack", 10, 100);
	ECHECK(buf, small_s->str == small_s->stackbuf);
	/* case */
	s16_realloc(small_s, 200);
	sprintf(buf, "small_s s16_realloc(%d) > stack max size(%d),no use stack", 200, 100);
	ECHECK(buf, small_s->str != small_s->stackbuf);
	/* case */
	s16_realloc(small_s, 99);
	sprintf(buf, "small_s s16_realloc(%d) < stack max size(%d),use stack", 99, 100);
	ECHECK(buf, small_s->str == small_s->stackbuf);
	/* case */
	s16_realloc(small_s, 100);
	sprintf(buf, "small_s s16_realloc(%d) == stack max size(%d),use stack", 100, 100);
	ECHECK(buf, small_s->str == small_s->stackbuf);
	/* case */
	s16_realloc(small_s, 101);
	sprintf(buf, "small_s s16_realloc(%d) > stack max size(%d),use stack", 101, 100);
	ECHECK(buf, small_s->str != small_s->stackbuf);
	s16_free(small_s);
	s16_free(ns);
	echeck_end();
	return 0;
}
