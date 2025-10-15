cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(myir)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror -Wno-invalid-offsetof")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

add_subdirectory(IR)

add_executable(My_IR main.cpp)
target_link_libraries(My_IR PRIVATE IR_lib)