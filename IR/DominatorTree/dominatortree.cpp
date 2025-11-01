#include "DominatorTree/dominatortree.hpp"
#include "DFS/rpo.hpp"
#include "Graph/graph.hpp"

#include <algorithm>
#include <iostream>

const std::vector<BasicBlock*> &DominatorTree::GetImmediateDominatedBlocks(BasicBlock* block) const {
    return immediateDominatedBlocks_.at(block);
}

std::vector<BasicBlock*> &DominatorTree::GetImmediateDominatedBlocks(BasicBlock* block) {
    return immediateDominatedBlocks_.at(block);
}

bool DominatorTree::IsDominatesOver(BasicBlock* domBlock, BasicBlock* block) const {
    if(block == domBlock) {
        return true;
    }
    auto it = std::find_if(dominatedBlocks_.at(domBlock).begin(), dominatedBlocks_.at(domBlock).end(),
                           [block](auto domBlock) { return domBlock == block; });
    return it != dominatedBlocks_.at(domBlock).end();
}

bool DominatorTree::Dominates(BasicBlock *dominator, BasicBlock *dominated) const {
    if(dominator == dominated) {
        return true;
    }
    
    auto it = dominatedBlocks_.find(dominator);
    if(it == dominatedBlocks_.end()) {
        return false;
    }
    
    const auto &dominatedSet = it->second;
    return std::find(dominatedSet.begin(), dominatedSet.end(), dominated) != dominatedSet.end();
}

void DominatorTree::Build() {
    RPO rpo{graph_};
    auto rpoVec = rpo.Run();

    auto *rootBlock = graph_->GetStartBlock();
    dominatedBlocks_[rootBlock] = rpoVec;

    for(auto it = rpoVec.begin(); it < rpoVec.end(); ++it) {
        dominatorsMap_.emplace(*it, std::vector<BasicBlock*> {rootBlock});
    }

    DFS dfs(graph_);

    for(auto blockIt = rpoVec.begin(); blockIt < rpoVec.end(); ++blockIt) {
        std::unordered_set<BasicBlock*> visitSet;
        visitSet.insert(*blockIt);
        auto reachableBlocks = dfs.Run(visitSet);

        CalculateDominatedBlocks(*blockIt, rpoVec, reachableBlocks);
    }

    for(auto blockIt = rpoVec.begin(); blockIt < rpoVec.end(); ++blockIt) {
        CalculateImmediateDominators(*blockIt);
    }
}

void DominatorTree::CalculateDominatedBlocks(BasicBlock* block, const std::vector<BasicBlock*> &originalVec,
                                             const std::vector<BasicBlock*> &reachableBlocks) {
    std::vector<BasicBlock*> dominatedBlocks;

    for(auto originalVecIt = originalVec.begin(); originalVecIt < originalVec.end(); ++originalVecIt) {
        auto dominatedBlock = std::find_if(
             reachableBlocks.begin(), reachableBlocks.end(),
             [&originalVecIt](auto reachableBlocksIt) -> bool { return reachableBlocksIt == *originalVecIt; });

        if(dominatedBlock == reachableBlocks.end() && *originalVecIt != block) {
            dominatedBlocks.push_back(*originalVecIt);
            auto mapIt = dominatorsMap_.find(*originalVecIt);
            if(mapIt != dominatorsMap_.end()) {
                mapIt->second.push_back(block);
            }
        }
    }

    dominatedBlocks_[block] = std::move(dominatedBlocks);
}

void DominatorTree::CalculateImmediateDominators(BasicBlock* block) {
    std::vector<BasicBlock*> immediateDominators;

    for(auto dominatedBlockIt: dominatedBlocks_[block]) {
        auto mapIt = dominatorsMap_.find(dominatedBlockIt);
        if(mapIt != dominatorsMap_.end()) {
           auto blocksDominatesOverCurrent = mapIt->second;
           auto it = std::find_if_not(blocksDominatesOverCurrent.begin(), blocksDominatesOverCurrent.end(),
                                      [block, this](auto domIt) { return IsDominatesOver(domIt, block); });
            if(it == blocksDominatesOverCurrent.end()) {
                immediateDominators.push_back(dominatedBlockIt);
            }
        }
    }

    immediateDominatedBlocks_[block] = std::move(immediateDominators);
}