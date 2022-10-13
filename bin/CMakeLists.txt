add_executable (mycutils.run ../src/main.c)

target_link_libraries (mycutils.run LINK_PUBLIC mycutils subproc)
