add_executable (subproc.run ../src/main.c)

target_link_libraries (subproc.run LINK_PUBLIC mycutils subproc)
