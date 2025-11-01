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

std::vector<std::pair<BasicBlock*, BasicBlock*>> DFS::RunLoopAnalyzer() {
    std::unordered_map<BasicBlock*, NodeColor> visitMap;
    std::vector<std::pair<BasicBlock*, BasicBlock*>> analyzerResult;
    BasicBlock* startBlock = graph_->GetStartBlock();

    DFSImpl(analyzerResult, visitMap, startBlock);
    return analyzerResult;
}

void DFS::DFSImpl(std::vector<std::pair<BasicBlock*, BasicBlock*>> &analyzerResult,
                  std::unordered_map<BasicBlock*, NodeColor> &visitMap, BasicBlock* block) {
    visitMap[block] = NodeColor::GREY;

    for(auto succBlock: block->GetSuccessors()) {
        if(visitMap.find(succBlock) == visitMap.end()) {
            DFSImpl(analyzerResult, visitMap, succBlock);
        }
        if(visitMap[succBlock] == NodeColor::GREY) {
            analyzerResult.push_back({succBlock, block});
        }
    }

    visitMap[block] = NodeColor::BLACK;
}