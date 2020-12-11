
# set the project name and version
cmake_minimum_required(VERSION 3.19)

project(Automata VERSION 1.0)
include_directories(.)
add_executable(main main.cpp)
add_executable(automata automata.cpp)
add_subdirectory(timed)
add_subdirectory(regex)
add_subdirectory(fsm)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
