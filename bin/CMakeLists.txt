add_executable (twitcam.run ../src/main.c)

target_link_libraries (twitcam.run LINK_PUBLIC subproc)
