
add_library (subproc ../../src/subproc.h ../../src/subproc.c)

target_link_libraries (subproc LINK_PUBLIC mycutils)

target_include_directories (subproc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
