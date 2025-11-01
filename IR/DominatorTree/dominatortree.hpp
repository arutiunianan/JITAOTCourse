#ifndef IR_DOMINATOR_TREE_H
#define IR_DOMINATOR_TREE_H

#include <vector>
#include <unordered_map>

class Graph;
class BasicBlock;

class DominatorTree final {
public:
    DominatorTree(Graph* graph): graph_(graph) {}
    const std::vector<BasicBlock*> &GetImmediateDominatedBlocks(BasicBlock* block) const;
    std::vector<BasicBlock*> &GetImmediateDominatedBlocks(BasicBlock* block);
    bool Dominates(BasicBlock* dominator, BasicBlock* dominated) const;

    void Build();

private:
    bool IsDominatesOver(BasicBlock* domblock, BasicBlock* block) const;

    void CalculateDominatedBlocks(BasicBlock* block, const std::vector<BasicBlock*> &originalVec,
                                  const std::vector<BasicBlock*> &reachableBlocks);

    void CalculateImmediateDominators(BasicBlock* block);

private:
    Graph* graph_ {nullptr};

    std::unordered_map<BasicBlock*, std::vector<BasicBlock*>> dominatorsMap_;
    std::unordered_map<BasicBlock*, std::vector<BasicBlock*>> dominatedBlocks_;
    std::unordered_map<BasicBlock*, std::vector<BasicBlock*>> immediateDominatedBlocks_;
};

#endif  // IR_DOMINATOR_TREE_H