#ifndef IR_LOOP_ANALYZER_HPP
#define IR_LOOP_ANALYZER_HPP

#include "Graph/graph.hpp"
#include "DominatorTree/dominatortree.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

class Loop;

class LoopAnalyzer final {
public:
    LoopAnalyzer(Graph* graph): graph_(graph), domTree_(graph) {}

    void Analyze();
    const std::vector<std::unique_ptr<Loop>>& GetLoops() const;
    void DumpLoops(std::ostream &ostr = std::cout) const;

private:
    void FindNaturalLoops();
    void BuildLoopTree();
    void ProcessBlock(BasicBlock *block, std::unordered_set<BasicBlock*>& visited);
    Loop *CreateNewLoop(BasicBlock *header, BasicBlock *backEdge);

private:
    Graph *graph_ = nullptr;

    DominatorTree domTree_;
    std::vector<std::unique_ptr<Loop>> loops_;
    std::unordered_map<BasicBlock*, Loop*> headerToLoop_;
};

class Loop final {
public:
    Loop(BasicBlock *header): header_(header) {}

    void AddBlock(BasicBlock *block);
    void AddBackEdge(BasicBlock *backEdge);
    void SetParent(Loop *parent);
    void AddSubLoop(Loop *subLoop);

    BasicBlock *GetHeader() const;
    const std::unordered_set<BasicBlock*>& GetBlocks() const;
    const std::vector<BasicBlock*>& GetBackEdges() const;
    Loop *GetParent() const;
    const std::vector<Loop*>& GetSubLoops() const;

    bool Contains(BasicBlock *block) const;

private:
    BasicBlock *header_ = nullptr;
    std::unordered_set<BasicBlock*> blocks_;
    std::vector<BasicBlock*> backEdges_;
    Loop *parent_ = nullptr;
    std::vector<Loop*> subLoops_;
};

#endif  // IR_LOOP_ANALYZER_HPP