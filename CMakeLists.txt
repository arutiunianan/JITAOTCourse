cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(myir)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wno-invalid-offsetof")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

add_executable(myir main.cpp

    IR/BasicBlock/basicblock.cpp
    IR/Graph/graph.cpp
    IR/Instr/dump.cpp
)

target_include_directories(myir PRIVATE ${CMAKE_SOURCE_DIR}/IR)