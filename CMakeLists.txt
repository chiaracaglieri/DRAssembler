cmake_minimum_required(VERSION 3.10)
project(Tesi)

set(CMAKE_CXX_STANDARD 11)

add_executable(Tesi parser.cpp data_structures.cpp state.cpp state.h interpreter.cpp state.h state.cpp dependencies.cpp dependencies.cpp util_dip.h util_dip.h util_dip.cpp)