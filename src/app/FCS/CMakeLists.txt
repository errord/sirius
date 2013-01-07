aux_source_directory(. SIRIUS_APP_FCS)
add_executable(fcs ${SIRIUS_APP_FCS})
target_link_libraries(fcs iconv sirius_base sirius_suffixtree)
add_dependencies(fcs sirius_suffixtree sirius_suffixtree_static sirius_base sirius_base_static)
