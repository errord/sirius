#ifndef _STRING16_H_
#define _STRING16_H_

typedef unsigned short _16b;

#define S16_STACKBUF_MAX 1024

size_t _16bslen(_16b *s);
_16b *_16bscpy(_16b *d, _16b *s);
_16b *_16bsdup(_16b *s);

#define ENCODE_GB2312 1
#define ENCODE_UNICODE 2

#define DEFINE_ENCODE ENCODE_UNICODE

#define S16_OUT_BUF_SIZE 2048

typedef struct {
	_16b *str;
	int encode;
	size_t len;
	size_t bufsize;
	size_t stacksize;
	_16b stackbuf[S16_STACKBUF_MAX];
} string16;

string16 *s16_new(size_t bufsize);

string16 *s16_stacksize_new(size_t stacksize, size_t bufsize);

/* clear s16 resource and s16 self */
void s16_free(string16 *s16);

/* init s16 resource */
void s16_resource_init(string16 *s16);

/* clear s16 resource */
void s16_resource_clear(string16 *s16);

/* set string encode */
void s16_set_encode(string16 *s16, int ecode);

/* get string encode */
int s16_get_encode(string16 *s16);

/* Duplicate str, need free return point */
string16 *s16_dup(string16 *s16);

/* realloc new size to s16, and copy old data to new s16 */
_16b *s16_realloc(string16 *s16, size_t size);

size_t s16_len(string16 *s16);

size_t s16_bufsize(string16 *s16);

size_t s16_stacksize(string16 *s16);

void s16_setlen(string16 *s16, size_t len);

/* recount s16 length, return new length */
size_t s16_recountlen(string16 *s16);

/* Duplicate s to str, need free s16 */
_16b *s16_set16bstr(string16 *s16, _16b *s);

/* Duplicate str to new memory, need free return point */
_16b *s16_get16bstr(string16 *s16);

/* Duplicate c str to string16, need free s16 */
_16b *s16_setcstr(string16 *s16, char *s);

/* return c str point, need free return point */
char *s16_tocstr_new(string16 *s16);

/* return c str point is buf */
char *s16_tocstr(string16 *s16, char *buf);

/* return str inner point */
_16b *s16_getstr(string16 *s16);

/* return s16 point, apps16 is string16, append apps16 to s16 */
string16 *s16_appends16(string16 *s16, string16 *apps16);

/* return s16 point, app16b is _16b, append app16b to s16 */
string16 *s16_append16b(string16 *s16, _16b app16b);

/* __16b to c str into buff, return buff point */
char *s16_16btocstr(_16b __16b, char *buf);

/* out 16b to buff */
char *s16b_out_to_buff(_16b b, char *buf, int encode);

/* out s16 to buff , need free return ptr */
char *s16_out_to_buff(string16 *s16);

/* out 16b to stdout */
void s16b_out(_16b b, int encode);

/* out s16 to stdout */
void s16_out(string16 *s16);

#endif /* _STRING16_H_ */

