cmake_minimum_required(VERSION 3.10)

project(setcover)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(HEADERS
    include/Instance.h
    include/Reducer.h
    include/MinHeap.h
    include/MaxHeap.h
    include/Solver.h)

set(SOURCES
    src/main.cpp
 )

add_executable(setcover ${SOURCES} ${HEADERS})
add_executable(preprocessor src/preprocess.cpp)