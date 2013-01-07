include_directories(.)
include_directories(${INCLUDE_OUTPUT_PATH})
add_library(easycheck easycheck.c)

# test_list
add_executable(test_list test_list.c)
target_link_libraries(test_list sirius_base easycheck iconv)

# test_sarray
add_executable(test_sarray test_sarray.c)
target_link_libraries(test_sarray sirius_base easycheck iconv)

# test_string16
add_executable(test_string16 test_string16.c)
target_link_libraries(test_string16 sirius_base easycheck iconv)

# test_suffixtree
add_executable(test_suffixtree test_suffixtree.c)
target_link_libraries(test_suffixtree easycheck iconv sirius_base sirius_suffixtree iconv)

add_dependencies(test_list easycheck sirius_suffixtree sirius_suffixtree_static sirius_base sirius_base_static)
add_dependencies(test_sarray easycheck sirius_suffixtree sirius_suffixtree_static sirius_base sirius_base_static)
add_dependencies(test_string16 easycheck sirius_suffixtree sirius_suffixtree_static sirius_base sirius_base_static)
add_dependencies(test_suffixtree easycheck sirius_suffixtree sirius_suffixtree_static sirius_base sirius_base_static)
