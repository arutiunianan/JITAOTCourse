#ifndef IR_DOMINATOR_TREE_HPP
#define IR_DOMINATOR_TREE_HPP

#include "DFS/rpo.hpp"

#include <vector>
#include <unordered_map>

class Graph;
class BasicBlock;

class DominatorTree final {
public:
    DominatorTree(Graph* graph): graph_(graph) {}

    void Build();

private:
    void CalculateDominatedBlocks(BasicBlock* block, const std::vector<BasicBlock*> &originalVec,
                                  const std::vector<BasicBlock*> &reachableBlocks);

    void CalculateImmediateDominators(BasicBlock* block);

    bool IsDominatesOver(BasicBlock *domIt, BasicBlock *block) const;

public:
    Graph* graph_ = nullptr;

    std::unordered_map<BasicBlock*, std::vector<BasicBlock*>> dominatorsMap_;
    std::unordered_map<BasicBlock*, std::vector<BasicBlock*>> dominatedBlocks_;
    std::unordered_map<BasicBlock*, std::vector<BasicBlock*>> immediateDominatedBlocks_;
};

#endif  // IR_DOMINATOR_TREE_HPP