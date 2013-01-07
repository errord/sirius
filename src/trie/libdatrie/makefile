CC = gcc
FLAG = -g -Wall
CLIB_PATH=..
INCLUDEPATH=-I$(CLIB_PATH) -I$(CLIB_PATH)/libcomm -I$(CLIB_PATH)/libcheck -I$(CLIB_PATH)/wordbase


all: libdatrie test

test: clean_o clean_a clean_test libdatrie
	$(CC) -o test $(FLAG) $(INCLUDEPATH) test.c -L. -ldatrie

libdatrie: clean_o clean_a
	$(CC) -c $(FLAG) $(INCLUDEPATH) $(CLIB_PATH)/libcheck/check.c
	$(CC) -c $(FLAG) $(INCLUDEPATH) $(CLIB_PATH)/wordbase/wordbase.c
	$(CC) -c $(FLAG) $(INCLUDEPATH) $(CLIB_PATH)/libcomm/libtime.c
	$(CC) -c $(FLAG) $(INCLUDEPATH) dictdatrie.c
	$(CC) -c $(FLAG) $(INCLUDEPATH) wordimage.c
	$(CC) -c $(FLAG) $(INCLUDEPATH) trie.c
	$(CC) -c $(FLAG) $(INCLUDEPATH) libdatrie.c
	$(CC) -c $(FLAG) $(INCLUDEPATH) datrie.c
	$(CC) -c $(FLAG) $(INCLUDEPATH) trie_out_text.c
	ar csr libdatrie.a *.o

clean_o:
	rm -rf *.o

clean_a:
	rm -rf *.a

clean_test:
	rm -rf test

clean: clean_o clean_a clean_test
