
# set the project name and version
cmake_minimum_required(VERSION 3.14)

project(Automata VERSION 1.0)
include_directories(.)
set (SRC main.cpp automata automata.cpp) 
add_subdirectory(timed)
add_subdirectory(regex)
add_subdirectory(fsm)

file(GLOB_RECURSE SRCS regex/*.cpp timed/*.cpp fsm/*.cpp automata.cpp main.cpp)
file(GLOB_RECURSE HEADER regex/*.h timed/*.h fsm/*.h automatas.h automata.h)
ADD_EXECUTABLE(Automata  ${SRCS} ${HEADER})


# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
