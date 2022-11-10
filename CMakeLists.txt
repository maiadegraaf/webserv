cmake_minimum_required(VERSION 3.23)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(.)

add_executable(webserv
        src/main.cpp src/webserv.h src/location.hpp src/config.hpp src/location.cpp src/config.cpp src/configParser.cpp src/configParser.hpp src/utils.cpp)