#ifndef IR_DFS_HPP
#define IR_DFS_HPP

#include "BasicBlock/basicblock.hpp"
#include "DFS/nodemarker.hpp"

#include <vector>
#include <unordered_set>
#include <unordered_map>

class Graph;

class DFS final {
public:
    DFS(Graph* graph): graph_(graph) {}

    std::vector<BasicBlock*> Run();
    std::vector<BasicBlock*> Run(std::unordered_set<BasicBlock*> &visitSet);
    std::vector<std::pair<BasicBlock*, BasicBlock*>> RunLoopAnalyzer();
private:
    void DFSImpl(std::vector<BasicBlock*> &dfsVector, std::unordered_set<BasicBlock*> &visitSet,
                 BasicBlock* block);
    void DFSImpl(std::vector<std::pair<BasicBlock*, BasicBlock*>> &analyzerResult,
                 std::unordered_map<BasicBlock*, NodeColor> &visitMap, BasicBlock* block);

private:
    Graph *graph_ = nullptr;
};

#endif  // IR_DFS_HPP