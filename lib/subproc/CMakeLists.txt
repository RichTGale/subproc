
add_library (subproc ../../src/subproc.h ../../src/subproc.c)

target_include_directories (subproc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
