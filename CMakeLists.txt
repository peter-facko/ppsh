cmake_minimum_required(VERSION 3.8)

project(PPshell)

add_subdirectory(PPshell)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17 -pedantic -Wall -Wextra -Werror")
