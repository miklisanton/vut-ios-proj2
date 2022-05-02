cmake_minimum_required(VERSION 3.17.3)
project(proj2 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -std=gnu99 -Wall -Wextra -Werror -pedantic ")

add_executable(proj2 main.c processes.c processes.h resources.c resources.h help.c help.h)
