#include "DominatorTree/dominatortree.hpp"
#include "DFS/rpo.hpp"
#include "Graph/graph.hpp"

#include <algorithm>
#include <iostream>

bool DominatorTree::IsDominatesOver(BasicBlock *domIt, BasicBlock *block) const {
    if (block == domIt) {
        return true;
    }
    
    const auto& dominated = dominatedBlocks_.at(domIt);
    auto it = std::find_if(dominated.begin(), dominated.end(),
                           [block](BasicBlock* domBlock) { return domBlock == block; });
    return it != dominated.end();
}

void DominatorTree::Build() {
    RPO rpo(graph_);
    std::vector<BasicBlock*> rpoVector = rpo.Run();

    auto *rootBlock = graph_->GetStartBlock();

    for (auto it = rpoVector.begin() + 1; it < rpoVector.end(); ++it) {
        dominatorsMap_.emplace(*it, std::vector<BasicBlock *> {rootBlock});
    }

    DFS dfs(graph_);

    for (auto blockIt = rpoVector.begin() + 1; blockIt < rpoVector.end(); ++blockIt) {
        auto reachableBlocks = dfs.Run();

        CalculateDominatedBlocks(*blockIt, rpoVector, reachableBlocks);
    }

    for (auto blockIt = rpoVector.begin(); blockIt < rpoVector.end(); ++blockIt) {
        CalculateImmediateDominators(*blockIt);
    }
}

void DominatorTree::CalculateDominatedBlocks(BasicBlock *block, const std::vector<BasicBlock *> &originalVec,
                                             const std::vector<BasicBlock *> &reachableBlocks) {
    std::vector<BasicBlock *> dominatedBlocks;

    for (auto originalVecIt = originalVec.begin(); originalVecIt < originalVec.end(); ++originalVecIt) {
        auto dominatedBlock = std::find_if(
            reachableBlocks.begin(), reachableBlocks.end(),
            [&originalVecIt](BasicBlock* reachableBlocksIt) -> bool { return reachableBlocksIt == *originalVecIt; });

        if (dominatedBlock == reachableBlocks.end() && *originalVecIt != block) {
            dominatedBlocks.push_back(*originalVecIt);
            auto mapIt = dominatorsMap_.find(*originalVecIt);
            if (mapIt != dominatorsMap_.end()) {
                mapIt->second.push_back(block);
            }
        }
    }

    dominatedBlocks_[block] = std::move(dominatedBlocks);
}

void DominatorTree::CalculateImmediateDominators(BasicBlock *block) {
    std::vector<BasicBlock *> immediateDominators;

    // Использовать at() вместо operator[] для const map
    for (auto dominatedBlockIt : dominatedBlocks_.at(block)) {
        auto mapIt = dominatorsMap_.find(dominatedBlockIt);
        if (mapIt != dominatorsMap_.end()) {
            auto blocksDominatesOverCurrent = mapIt->second;
            // Захватить this для вызова метода IsDominatesOver
            auto it = std::find_if_not(blocksDominatesOverCurrent.begin(), blocksDominatesOverCurrent.end(),
                [this, block](BasicBlock* domIt) { return IsDominatesOver(domIt, block); });
            if (it == blocksDominatesOverCurrent.end()) {
                immediateDominators.push_back(dominatedBlockIt);
            }
        }
    }

    immediateDominatedBlocks_[block] = std::move(immediateDominators);
}