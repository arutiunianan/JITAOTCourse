#include "dfs.hpp"
#include "Graph/graph.hpp"

std::vector<BasicBlock*> DFS::Run()
{
    std::vector<BasicBlock*> dfsVector;
    std::unordered_set<BasicBlock*> visitSet;

    DFSImpl(dfsVector, visitSet, graph_->GetStartBlock());

    return dfsVector;
}

void DFS::DFSImpl(std::vector<BasicBlock*> &dfsVector, std::unordered_set<BasicBlock*> &visitSet,
                  BasicBlock* block)
{
    visitSet.insert(block);
    dfsVector.push_back(block);

    for(auto succBlock: block->GetSuccessors()) {
        if(visitSet.find(succBlock) == visitSet.end()) {
            DFSImpl(dfsVector, visitSet, succBlock);
        }
    }
}