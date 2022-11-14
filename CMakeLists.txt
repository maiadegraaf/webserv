cmake_minimum_required(VERSION 3.23)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(.)

add_executable(webserv
        src/main.cpp src/configParser.cpp src/utils/utils_Maia.cpp)