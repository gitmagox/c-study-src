cmake_minimum_required(VERSION 3.16)
project(processpool C)

set(CMAKE_C_STANDARD 99)

add_executable(processpool main.c select_event.h map.c map.h timer_wheel.h)