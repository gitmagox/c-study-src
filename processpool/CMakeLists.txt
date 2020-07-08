cmake_minimum_required(VERSION 3.15)
project(processpool C)

set(CMAKE_C_STANDARD 99)

add_executable(processpool main.c select_event.h map.c map.h timer_wheel.h Interface/timer.h tcp_connection.h hash_map.h magox.h Interface/protocol.h Protocol/http.h Interface/connection.h Interface/base.h Protocol/protocol_factory.h)