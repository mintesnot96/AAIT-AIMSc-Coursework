cmake_minimum_required(VERSION 3.26)
project(lifepthread C)

set(CMAKE_C_STANDARD 11)

add_executable(lifepthread life.c
        plot.c
        real_rand.c
        timer.c
        life.c)
