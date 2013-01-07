#ifndef _SICONV_H_
#define _SICONV_H_

/*
 * simpleness iconv
*/

#ifndef WIN32
#include <iconv.h>
#else
#define iconv_t int
iconv_t siconv_open(char *a, char *b);
size_t siconv(iconv_t i, char **in, size_t *is, char **oout, size_t *s);
void siconv_close(iconv_t i);
#define iconv_open siconv_open
#define iconv siconv
#define iconv_close siconv_close
#endif /* WIN32 */


#endif /* _SICONV_H_ */

