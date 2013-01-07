#include <stdlib.h>
#include <stdio.h>
#include <string16.h>
#include <utf16_to_gb2312.h>

#define USE_STACK(s16, size) (size <= s16->stacksize)
#define NO_USE_STACK(s16, size) (!USE_STACK(s16, size))
#define BUF_USE_STACK(s) (s->bufsize <= s->stacksize)
#define BUF_NO_USE_STACK(s) (!BUF_USE_STACK(s))

size_t _strlen(char *s)
{
	size_t i;
	for (i = 0; s[i]; ++i) ;
	return i;
}

size_t _16bslen(_16b *s)
{
	size_t i;
	for (i = 0; s[i]; ++i) ;
	return i;
}

_16b *_16bscpy(_16b *d, _16b *s)
{
	_16b *o = d;
	for (; (*d=*s); ++d, ++s) ;
	return o;
}

_16b *_16bnscpy(_16b *d, _16b *s, size_t n)
{
	_16b *o = d;
	for (; (*d=*s) && n>0; ++d, ++s, --n) ;
	return o;
}

_16b *_16bsdup(_16b *s)
{
	_16b* d;
	d = (_16b*)malloc(sizeof(_16b) * (_16bslen(s)+1));
	return _16bscpy(d, s);
}

static _16b *_alloc(string16 *s16, size_t size)
{
	if (size == 0) {
		s16->str = NULL;
	} else if (NO_USE_STACK(s16, size)) {
		s16->str = (_16b*)malloc(sizeof(_16b) * size);
	} else {
		s16->str = s16->stackbuf;
	}
	s16->len = 0;
	s16->bufsize = size;
	return s16->str;
}

static void _free(string16 *s16)
{
	if (BUF_NO_USE_STACK(s16))
		free(s16->str);
	s16->str = NULL;
}

static _16b *_realloc_empty(string16 *s16, size_t size)
{
	s16_resource_clear(s16);
	return _alloc(s16, size);
}

static _16b *_realloc(string16 *s16, size_t size)
{
	_16b *ptr;
	size_t len;
	if (size == 0) {
		_free(s16);
		len = 0;
		ptr = NULL;
	} else if (NO_USE_STACK(s16, size)) {
		ptr = (_16b*)malloc(sizeof(_16b) * size+1);
		len = size > s16->len ? s16->len : size;
		if (len > 0)
			_16bnscpy(ptr, s16->str, len);
		_free(s16);
	} else {
		/* new length */
		len = size > s16->len ? s16->len : size;
		/* old string no use stack , copy to stack */
		if (BUF_NO_USE_STACK(s16)) {
			if (len > 0)
				_16bnscpy(s16->stackbuf, s16->str, len);
		}
		_free(s16);
		ptr = s16->stackbuf;
	}
	s16->str = ptr;
	s16->len = len;
	s16->bufsize = size;
	return s16->str;
}

static void _set_define_encode(string16 *s16)
{
	s16->encode = DEFINE_ENCODE;
}

string16 *s16_new(size_t bufsize)
{
	string16 *s16;
	s16 = (string16*)malloc(sizeof(string16));
	if (s16 == NULL)
		return NULL;
	s16->stacksize = S16_STACKBUF_MAX;
	_alloc(s16, bufsize);
	_set_define_encode(s16);
	return s16;
}

string16 *s16_stacksize_new(size_t stacksize, size_t bufsize)
{
	string16 *s16;
	size_t size = sizeof(string16) - sizeof(_16b) * S16_STACKBUF_MAX + sizeof(_16b) * stacksize;
	s16 = (string16*)malloc(size);
	if (s16 == NULL)
		return NULL;
	s16->stacksize = stacksize;
	_alloc(s16, bufsize);
	_set_define_encode(s16);
	return s16;
}

void s16_resource_init(string16 *s16)
{
	s16->str =NULL;
	s16->len = 0;
	s16->bufsize = 0;
	s16->stacksize = S16_STACKBUF_MAX;
	_set_define_encode(s16);
}

string16 *s16_dup(string16 *s16)
{
	string16 *des;
	des = s16_new(s16->len+1);
	_16bscpy(des->str, s16->str);
	return des;
}

_16b *s16_realloc(string16 *s16, size_t size)
{
	return _realloc(s16, size);
}

void s16_resource_clear(string16 *s16)
{
	if (s16->str != NULL)
		_free(s16);
	s16->str = NULL;
	s16->len = 0;
	s16->bufsize = 0;
}

void s16_set_encode(string16 *s16, int ecode)
{
	s16->encode = ecode;
}

int s16_get_encode(string16 *s16)
{
	return s16->encode;
}

void s16_free(string16 *s16)
{
	s16_resource_clear(s16);
	free(s16);
}

size_t s16_len(string16 *s16)
{
	return s16->len;
}

size_t s16_bufsize(string16 *s16)
{
	return s16->bufsize;
}

void s16_setlen(string16 *s16, size_t len)
{
	s16->len = len;
}

size_t s16_stacksize(string16 *s16)
{
	return s16->stacksize;
}

size_t s16_recountlen(string16 *s16)
{
	s16->len = _16bslen(s16->str);
	return s16->len;
}

_16b *s16_set16bstr(string16 *s16, _16b *s)
{
	size_t len = _16bslen(s) + 1;
	_realloc_empty(s16, len);
	_16bscpy(s16->str, s);
	s16->len = len - 1;
	return s16->str;
}

_16b *s16_get16bstr(string16 *s16)
{
	return _16bsdup(s16->str);
}

_16b *s16_setcstr(string16 *s16, char *s)
{
	size_t len;
	size_t i, n;
	_16b b;
	len = _strlen(s);
	_realloc_empty(s16, len+1);
	for (i = 0, n = 0; i < len; ++i, ++n) {
		if ((unsigned char)s[i] > 0x80) {
                        *((char*)&b) = s[i];
                        *((char*)&b + 1) = s[++i];
		} else {
			b = s[i];
		}
		s16->str[n] = b;
	}
	s16->len = n;
	s16->str[n] = 0;
	return s16->str;
}

static char *_tocstr(string16 *s16, char *buf)
{
	size_t len;
	size_t i, n;
	_16b b;
	char c1, c2;
	len = s16_recountlen(s16);
	if (buf == NULL)
		buf = (char*)malloc(sizeof(char) * (len+1) * 2);
	for (i = 0, n = 0; i < len; ++i, ++n) {
		b = s16->str[i];
                c1 = *((char*)&b);
		c2 = *((char*)&b + 1);
		if (c1 < 0) {	
                        buf[n] = c1;
                        buf[++n] = c2;
                } else {
                        buf[n] = c1;
                }
	}
	buf[n] = 0;
	return buf;
}

char *s16_tocstr_new(string16 *s16)
{
	return _tocstr(s16, NULL);
}

char *s16_tocstr(string16 *s16, char *buf)
{
	return _tocstr(s16, buf);
}


_16b *s16_getstr(string16 *s16)
{
	return s16->str;
}

string16 *s16_appends16(string16 *s16, string16 *apps16)
{
	size_t len;
	if (apps16 == NULL || apps16->str == NULL)
		return s16;

	len = s16->len + apps16->len;
	if (len > s16->bufsize) {
		s16_realloc(s16, len << 1);
	}
	_16bscpy(s16->str + s16->len, apps16->str);
	s16->len = len;
	return s16;
}

string16 *s16_append16b(string16 *s16, _16b app16b)
{
	size_t len;
	if (app16b == 0)
		return NULL;
	len = s16->len + 1;
	if (len >= s16->bufsize)
		s16_realloc(s16, len << 1); /* x2 */
	s16->str[s16->len] = app16b;
	s16->len++;
	s16->str[s16->len] = '\0';
	return s16;
}

char *s16_16btocstr(_16b __16b, char *buf)
{
	char c1, c2;
	c1 = *((char*)&__16b);
	c2 = *((char*)&__16b + 1);
	if (c1 < 0) {	
		buf[0] = c1;
		buf[1] = c2;
		buf[2] = 0;
	} else {
		buf[0] = c1;
		buf[1] = 0;
	}
	return buf;
}

/* TODO out utf16 */
char *s16b_out_to_buff(_16b b, char *buf, int encode)
{
	if (b == 0)
		return NULL;
	if (encode == ENCODE_GB2312) {
		return s16_16btocstr(b, buf);
	}
	else if (encode == ENCODE_UNICODE) {
		//out = utf16_to_gb2312(s16, buf, 3);
	}
	return NULL;
}

char *s16_out_to_buff(string16 *s16)
{
	char *buf = NULL;
	int encode;
	size_t len;

	len = s16_len(s16);
	if (len == 0)
		return NULL;
	len++;
	len *= 2;
	buf = (char*)malloc(sizeof(char) * len);

	encode = s16_get_encode(s16);
	if (encode == ENCODE_GB2312)
		return s16_tocstr(s16, buf);
	else if (encode == ENCODE_UNICODE)
		return utf16_to_gb2312(s16, buf, len);
	free(buf);
	return NULL;
}


void s16b_out(_16b b, int encode)
{
	char buf[3];
	printf("%s", s16b_out_to_buff(b, buf, encode));
}

void s16_out(string16 *s16)
{
	char buf[S16_OUT_BUF_SIZE];
	char *new_str = NULL;
	char *out;
	int encode;
	size_t len;

	len = s16_len(s16);
	if (len == 0)
		return ;
	len++;
	len *= 2;
	if (len > S16_OUT_BUF_SIZE) {
		new_str = (char*)malloc(sizeof(char) * len);
		out = new_str;
	}
	else {	
		out = buf;
		len = S16_OUT_BUF_SIZE;
	}	

	encode = s16_get_encode(s16);
	if (encode == ENCODE_GB2312)
		out = s16_tocstr(s16, out);
	else if (encode == ENCODE_UNICODE)
		out = utf16_to_gb2312(s16, out, len);
	printf("%s", out);
	if (new_str != NULL)
		free(new_str);
}
