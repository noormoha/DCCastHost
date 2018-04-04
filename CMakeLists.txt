cmake_minimum_required(VERSION 3.9)


project(DCCastHost)

set(CMAKE_CXX_STANDARD 11)

add_library(NormDefinition NORM/NormDefinition.h NORM/NormDefinition.cpp)

add_library(NormReceiver NORM/NormReceiver.h NORM/NormReceiver.cpp)
target_link_libraries(NormReceiver NormDefinition norm pthread)

add_library(NormSender NORM/NormSender.h NORM/NormSender.cpp)
target_link_libraries(NormSender NormDefinition norm pthread)

add_library(NormInstance NORM/NormInstance.h NORM/NormInstance.cpp)


add_executable(DCCastHost
        main.cpp
        json.hpp
        REST/cleanAll.cpp
        REST/progress.cpp
        REST/activeTransfer.cpp
        REST/terminate.cpp REST/API.h REST/updateRate.cpp REST/receive.cpp REST/send.cpp)

target_link_libraries(DCCastHost pistache pthread)

add_executable(test_receiver test/test_receiver.cpp)
target_link_libraries(test_receiver NormReceiver)

add_executable(test_sender test/test_sender.cpp)
target_link_libraries(test_sender NormSender)
