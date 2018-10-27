cmake_minimum_required(VERSION 3.5)
project(mona)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -g")

aux_source_directory(./mona/base base)
aux_source_directory(./mona/net net)
include_directories(${base} ${net})


add_executable(mona main.cpp ${base} ${net})

