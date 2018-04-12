cmake_minimum_required(VERSION 3.9)


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

target_link_libraries(DCCastHost pistache pthread NormManager)

# TEST EXECUTABLES

add_executable(test_receiver test/test_receiver.cpp)
target_link_libraries(test_receiver NormReceiver)

add_executable(test_sender test/test_sender.cpp)
target_link_libraries(test_sender NormSender)

add_executable(test_sender_instance test/test_norm_instance_sender.cpp)
target_link_libraries(test_sender_instance NormInstance)

add_executable(test_receiver_instance test/test_norm_instance_receiver.cpp)
target_link_libraries(test_receiver_instance NormInstance)

add_executable(test_manager_receive test/test_manager_receive.cpp)
target_link_libraries(test_manager_receive NormManager)

add_executable(test_manager_send test/test_manager_send.cpp)
target_link_libraries(test_manager_send NormManager)

add_executable(test_active_receiver test/test_active_receiver.cpp)
target_link_libraries(test_active_receiver NormManager)

add_executable(test_active_sender test/test_active_sender.cpp)
target_link_libraries(test_active_sender NormManager)

set(mininet_rates 100000000 400000000 1000000000 2000000000)

foreach(rate ${mininet_rates})
    add_executable(test_mininet_sender_${rate} test/test_mininet_sender.cpp)
    target_link_libraries(test_mininet_sender_${rate} NormSender)
    target_compile_definitions(test_mininet_sender_${rate} PRIVATE SENDER_RATE=${rate})
endforeach()

add_executable(test_mininet_receiver test/test_mininet_receiver.cpp)
target_link_libraries(test_mininet_receiver NormReceiver)


