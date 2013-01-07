#ifndef _LIB_TEXT_FILTER_H_
#define _LIB_TEXT_FILTER_H_

/*
 * load filter dict
 * return dict handle
 * return 0 if failed
 */
int tf_load_filterdict(const char* dict);

/*
 * unload filter dict
 * 
*/
void tf_unload_filterdict(int handle);

/*
 * text filter
 */
void tf_filter(int handle, const char* text);

#endif /* _LIB_TEXT_FILTER_H_ */

