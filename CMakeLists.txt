cmake_minimum_required(VERSION 3.8)

project(PPshell)

add_subdirectory(PPshell)

#add_custom_target(
#    cstyle
#    COMMAND find ${CMAKE_CURRENT_SOURCE_DIR}/ -name \"*.[ch]\" -exec clang-format -style=file -i {} "\;"
#)
#add_dependencies(PPshell cstyle)
