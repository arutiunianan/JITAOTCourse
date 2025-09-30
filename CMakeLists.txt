cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(myir)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror -Wno-invalid-offsetof")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

add_library(IR_lib STATIC
    IR/BasicBlock/basicblock.cpp
    IR/Graph/graph.cpp
    IR/Instr/dump.cpp
    IR/Instr/instruction.cpp
)

target_include_directories(IR_lib PUBLIC ${CMAKE_SOURCE_DIR}/IR)

add_executable(My_IR main.cpp)

target_link_libraries(My_IR PRIVATE IR_lib)