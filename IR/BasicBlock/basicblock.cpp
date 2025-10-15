#include "BasicBlock/basicblock.hpp"

#include <iomanip>

void BasicBlock::PushInstruction(Instruction* instr) {
    if (lastInstr_ == nullptr) {
        firstInstr_ = instr;
        lastInstr_ = firstInstr_;
    } else {
        lastInstr_->SetNext(instr);
        instr->SetPrev(lastInstr_);
        lastInstr_ = instr;
    }
}

void BasicBlock::SetId(size_t id) {
    bbId_ = id;
}
size_t BasicBlock::GetId() const {
    return bbId_;
}

void BasicBlock::SetGraph(Graph* graph) {
    graph_ = graph;
}

void BasicBlock::AddSuccessor(BasicBlock* block) {
    successors_.push_back(block);
}

void BasicBlock::AddPredecessor(BasicBlock* block) {
    predecessors_.push_back(block);
}

const std::vector<BasicBlock*>& BasicBlock::GetSuccessors() const {
    return successors_;
}

const std::vector<BasicBlock*>& BasicBlock::GetPredecessors() const {
    return predecessors_;
}

Instruction* BasicBlock::GetLastInstr() const {
    return lastInstr_;
}

void BasicBlock::Dump(std::stringstream &ss) const
{
    ss << "BB_" << bbId_ << ":\n";

    auto currInstr = firstInstr_;
    while (currInstr != nullptr) {
        ss << std::setw(4);
        currInstr->Dump(ss);
        ss << "\n";
        currInstr = currInstr->GetNext();
    }
}