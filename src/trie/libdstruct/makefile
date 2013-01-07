#
# libdstruct makefile
#
SRC = ./src
OUT_BIN_PATH = ./bin
OUT_LIB_PATH = ./lib
OUT_INCLUDE_PATH = ./include
IN_LIB_PATH = -L$(OUT_LIB_PATH)
IN_INCLUDE_PATH = -I./
CC = g++
DEBUG_FLAG = -g
CC_FLAG = -c $(DEBUG_FLAG) -W -fPIC

all: clean dir libdstruct tool test

dir:
	mkdir $(OUT_BIN_PATH)
	mkdir $(OUT_LIB_PATH)
	mkdir $(OUT_INCLUDE_PATH)

libdstruct:
	$(CC) $(CC_FLAG) $(IN_INCLUDE_PATH) -c $(SRC)/baseio.cpp
	$(CC) $(CC_FLAG) $(IN_INCLUDE_PATH) -c $(SRC)/dstruct.cpp
	$(CC) $(CC_FLAG) $(IN_INCLUDE_PATH) -c $(SRC)/dsioformat.cpp
	ar csr libdstruct.a baseio.o dstruct.o dsioformat.o
	rm -rf baseio.o dstruct.o dsioformat.o
	mv libdstruct.a $(OUT_LIB_PATH)
	cp $(SRC)/*.h $(OUT_INCLUDE_PATH)

tool:  $(SRC)/ds2xml.cpp $(SRC)/xml2ds.cpp
	$(CC) $(CC_FLAG) $(IN_INCLUDE_PATH) $(SRC)/ds2xml.cpp
	$(CC) $(CC_FLAG) $(IN_INCLUDE_PATH) $(SRC)/xml2ds.cpp
	#ds2xml
	$(CC) -o ds2xml ds2xml.o $(IN_LIB_PATH) -ldstruct
	#xml2ds
	$(CC) -o xml2ds xml2ds.o $(IN_LIB_PATH) -ldstruct
	rm -rf ds2xml.o
	rm -rf xml2ds.o
	mv ds2xml $(OUT_BIN_PATH)
	mv xml2ds $(OUT_BIN_PATH)

clean:
	rm -rf *.o
	rm -rf $(OUT_BIN_PATH)
	rm -rf $(OUT_LIB_PATH)
	rm -rf $(OUT_INCLUDE_PATH)

test:
	$(CC) $(CC_FLAG) $(IN_INCLUDE_PATH) -c $(SRC)/easycheck.cpp
	$(CC) $(CC_FLAG) $(IN_INCLUDE_PATH) -c $(SRC)/test_dstruct.cpp
	$(CC) -o test_dstruct easycheck.o test_dstruct.o $(IN_LIB_PATH) -ldstruct
	rm -rf easycheck.o test_dstruct.o
	mv test_dstruct $(OUT_BIN_PATH)

runtest:
	$(OUT_BIN_PATH)/test_dstruct