find_package(GTest REQUIRED)

add_executable(IR_tests main.cpp dominatortree.cpp)

target_include_directories(IR_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/IR
    ${CMAKE_SOURCE_DIR}/IR/BasicBlock
    ${CMAKE_SOURCE_DIR}/IR/DFS
    ${CMAKE_SOURCE_DIR}/IR/DominatorTree
    ${CMAKE_SOURCE_DIR}/IR/Graph
    ${CMAKE_SOURCE_DIR}/IR/Instr
)

target_link_libraries(IR_tests PRIVATE 
    IR_lib
    GTest::GTest
    GTest::Main
)

add_test(NAME gtest_test COMMAND IR_tests)