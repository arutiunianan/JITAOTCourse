#ifndef IR_DFS_HPP
#define IR_DFS_HPP

#include "BasicBlock/basicblock.hpp"

#include <vector>
#include <unordered_set>

class Graph;

class DFS final {
public:
    DFS(Graph* graph): graph_(graph) {}

    std::vector<BasicBlock*> Run();
    std::vector<BasicBlock*> Run(std::unordered_set<BasicBlock*> &visitSet);
private:
    void DFSImpl(std::vector<BasicBlock*> &dfsVector, std::unordered_set<BasicBlock*> &visitSet,
                 BasicBlock* block);

private:
    Graph *graph_ = nullptr;
};

#endif  // IR_DFS_HPP