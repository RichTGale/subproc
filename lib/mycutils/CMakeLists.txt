
add_library (mycutils ../../src/mycutils.h ../../src/mycutils.c)

target_include_directories (mycutils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
