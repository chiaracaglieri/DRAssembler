cmake_minimum_required(VERSION 3.10)
project(Tesi)

set(CMAKE_CXX_STANDARD 11)

add_executable(Tesi parser.cpp data_structures.cpp int_util.cpp int_util.h interpreter.cpp int_util.h int_util.cpp dependencies.cpp dependencies.cpp dep_util.h dep_util.h dep_util.cpp)