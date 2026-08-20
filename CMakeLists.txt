
cmake_minimum_required(VERSION 3.16)
project(sysmon_lite LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
include_directories(include)
add_executable(sysmon
    src/main.cpp
    src/Monitor.cpp)
