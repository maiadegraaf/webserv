cmake_minimum_required(VERSION 3.23)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(.)

add_executable(webserv
        src/main.cpp src/config_parser.cpp includes/config_parser.h includes/webserv.h includes/location.hpp includes/config.hpp src/location.cpp src/config.cpp)