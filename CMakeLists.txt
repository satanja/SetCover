cmake_minimum_required(VERSION 3.10)

project(setcover)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(HEADERS
)

set(SOURCES
    src/main.cpp
)

add_executable(setcover ${SOURCES} ${HEADERS})