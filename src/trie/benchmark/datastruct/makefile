CC = g++
FLAG = -g
D = -DBENCHMARK
INCLUDE = -I../ -I../.. -I../benchmark
all:
	$(CC) $(FLAG) $(D) $(INCLUDE) hashfun.cpp memstate.cpp ../../libhash/hashfun.c ../../libcomm/libtime.c -o hashfun_benchmark
	$(CC) $(FLAG) $(D) $(INCLUDE) hash.cpp memstate.cpp ../../libhash/hashfun.c ../../libcomm/libtime.c \
	../../libhash/linehash/linehash.c \
	../../libhash/srilmHash/srilmHash.cc \
	../../libhash/stlhash/stlhash.cpp \
	../../libhash/fastdfshash/fastdfshash.c ../../libhash/fastdfshash/chain.c ../../libhash/fastdfshash/hash.c \
	../../libhash/mxhash/mxhash.c ../../libhash/mxhash/hashtable.c \
	./hash/hashtable.c \
	-o hash_benchmark


