#ifndef _SUFFIXTREE_CONFIG_H_
#define _SUFFIXTREE_CONFIG_H_

#define TEXTMATRIX_DEFAULT_ROW_SIZE 100
#define TEXTMATRIX_DEFAULT_COLUMN_SIZE 1000

/*
data size: 50219
string: abcdefg1234 .... abcdefg1234

no optimize:
time ./test

real	0m18.565s
user	0m18.475s
sys	0m0.049s

optimize:
time ./test

real	0m10.606s
user	0m10.555s
sys	0m0.038s
*/
#define GETTOKEN_OPTIMIZE

#endif /* _SUFFIXTREE_CONFIG_H_ */
