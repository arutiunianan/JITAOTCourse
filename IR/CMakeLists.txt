add_library(IR_lib STATIC
    irbuilder.cpp
    BasicBlock/basicblock.cpp
    Graph/graph.cpp
    Instr/dump.cpp
    Instr/instruction.cpp
    DFS/dfs.cpp
    DominatorTree/dominatortree.cpp
    LoopAnalyzer/loopanalyzer.cpp
)

target_include_directories(IR_lib PUBLIC ${CMAKE_SOURCE_DIR}/IR)