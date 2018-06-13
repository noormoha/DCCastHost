cmake_minimum_required(VERSION 3.5)


project(DCCastHost)

set(CMAKE_CXX_STANDARD 14)

add_library(NormDefinition NORM/NormDefinition.h NORM/NormDefinition.cpp)

add_library(NormReceiver NORM/NormReceiver.h NORM/NormReceiver.cpp)
target_link_libraries(NormReceiver NormDefinition norm pthread)

add_library(NormSender NORM/NormSender.h NORM/NormSender.cpp)
target_link_libraries(NormSender NormDefinition norm pthread)

add_library(NormInstance NORM/NormInstance.h NORM/NormInstance.cpp)
target_link_libraries(NormInstance NormSender NormReceiver)

add_library(NormManager NORM/NormManager.h NORM/NormManager.cpp)
target_link_libraries(NormManager NormInstance)


add_executable(DCCastHost
        main.cpp
        json.hpp
        REST/cleanAll.cpp
        REST/progress.cpp
        REST/activeTransfer.cpp
        REST/terminate.cpp REST/API.h REST/updateRate.cpp REST/receive.cpp REST/send.cpp)

add_executable(PistacheTest
        test/pistache_test.cpp)

target_link_libraries(PistacheTest pistache pthread)

target_link_libraries(DCCastHost pistache pthread NormManager)


