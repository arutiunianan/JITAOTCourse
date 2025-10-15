#include "Graph/graph.hpp"

void Graph::AddBlock(std::unique_ptr<BasicBlock> block) {
    size_t currblockNum = basicBlocks_.size();
    block->SetId(currblockNum);
    block->SetGraph(this);
    basicBlocks_.push_back(std::move(block));
}

BasicBlock* Graph::GetStartBlock() const {
    return basicBlocks_.front().get();
}

void Graph::AddInstruction(std::unique_ptr<Instruction> instr) {
    instr->SetId(instructions_.size());
    instructions_.push_back(std::move(instr));
}

void Graph::Dump(std::stringstream &ss) const {
    for (auto &bb : basicBlocks_) {
        bb->Dump(ss);
    }
}