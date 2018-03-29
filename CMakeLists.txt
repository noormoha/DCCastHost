cmake_minimum_required(VERSION 3.9)


project(DCCastHost)

set(CMAKE_CXX_STANDARD 11)

add_executable(DCCastHost
        main.cpp
        json.hpp
        REST/cleanAll.cpp
        REST/progress.cpp
        REST/activeTransfer.cpp
        REST/terminate.cpp REST/API.h REST/updateRate.cpp REST/receive.cpp REST/send.cpp)

target_link_libraries(DCCastHost pistache pthread)
