project(sirius)
cmake_minimum_required(VERSION 2.6)
# version
set(LIB_SIRIUS_BASE_VERSION 0.1)
set(LIB_SIRIUS_BASE_API_VERSION 0.2)
# bin path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 
# libray path
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib) 
# include path
set(INCLUDE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/include)
# mkdir include
file(MAKE_DIRECTORY ${INCLUDE_OUTPUT_PATH})
# sub directory
add_subdirectory(src)
# test
IF (TEST)
   add_subdirectory(src/test)
ENDIF(TEST)

