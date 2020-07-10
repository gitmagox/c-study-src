cmake_minimum_required(VERSION 3.15)
project(processpool C)


set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(Connection)
include_directories(Interface)
include_directories(Protocol)
include_directories(Protocol/Http)
include_directories(Timer)

add_executable(processpool
        Connection/tcp_connection.h
        Interface/base.h
        Interface/connection.h
        Interface/protocol.h
        Interface/protocol_message.h
        Interface/timer.h
        Protocol/Http/request.h
        Protocol/http.h
        Protocol/http_paser.h
        Protocol/protocol_factory.h
        Timer/timer_factory.c
        Timer/timer_factory.h
        Timer/timer_wheel.c
        Timer/timer_wheel.h
        hash_map.h
        main.c
        map.c
        map.h
        select_event.c
        select_event.h)

#TARGET_LINK_LIBRARIES(processpool
#            .
#            ./Timer/timer_wheel.c.o
#
#        )