cmake_minimum_required(VERSION 3.26)
project(lifeopenmp C)

set(CMAKE_C_STANDARD 11)

add_executable(lifeopenmp life.c
        life.c
        plot.c
        timer.c)
