#include "dfs.hpp"
#include "Graph/graph.hpp"

#include <iostream>

std::vector<BasicBlock*> DFS::Run() {
    std::unordered_set<BasicBlock*> visitSet;
    return Run(visitSet);
}

std::vector<BasicBlock*> DFS::Run(std::unordered_set<BasicBlock*> &visitSet) {
    std::vector<BasicBlock*> dfsVector;
    BasicBlock* startBlock = graph_->GetStartBlock();
    if(visitSet.find(startBlock) ==  visitSet.end()) {
        DFSImpl(dfsVector, visitSet, startBlock);
    }

    return dfsVector;
}

void DFS::DFSImpl(std::vector<BasicBlock*> &dfsVector, std::unordered_set<BasicBlock*> &visitSet,
                  BasicBlock* block) {
    visitSet.insert(block);
    dfsVector.push_back(block);

    for(auto succBlock: block->GetSuccessors()) {
        if(visitSet.find(succBlock) == visitSet.end()) {
            DFSImpl(dfsVector, visitSet, succBlock);
        }
    }
}