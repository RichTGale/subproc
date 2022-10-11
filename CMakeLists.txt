# CMakeLists files in this project can
# refer to the root source directory of the project as ${TWITCAM_SOURCE_DIR} and
# to the root binary directory of the project as ${TWITCAM_BINARY_DIR}.
cmake_minimum_required (VERSION 3.12)
project (TWITCAM)

add_subdirectory (lib/subproc)
add_subdirectory (bin)
