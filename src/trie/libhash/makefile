CC = gcc
CPP = g++
FLAG = -g

all: 
	$(CC) -o testlinehash $(FLAG) -I../ -I./ ./linehash/linehash.c hashfun.c ./linehash/testlinehash.c -W
	$(CPP) -o testsrihash $(FLAG) -I../ -I./ ./srilmHash/testsrilmhash.cc ./srilmHash/srilmHash.cc
	$(CPP) -o teststlhash $(FLAG) -I../ -I./ ./stlhash/teststlhash.cpp ./stlhash/stlhash.cpp
	$(CC) -o testdfhash $(FLAG) -I../ -I./ ./fastdfshash/testdfhash.c ./fastdfshash/fastdfshash.c ./fastdfshash/hash.c ./fastdfshash/chain.c
	$(CC) -o testmxhash $(FLAG) -I../ -I./ ./mxhash/testmxhash.c ./mxhash/mxhash.c ./mxhash/hashtable.c
