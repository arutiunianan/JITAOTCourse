#ifndef IR_BASIC_BLOCK_HPP
#define IR_BASIC_BLOCK_HPP

#include "Instr/instruction.hpp"

#include <vector>
#include <string>
#include <memory>
#include <sstream>

class Graph;

class BasicBlock final {
public:
    void PushInstruction(Instruction* instr) {
        if (lastInstr_ == nullptr) {
            firstInstr_ = instr;
            lastInstr_ = firstInstr_;
        } else {
            lastInstr_->SetNext(instr);
            instr->SetPrev(lastInstr_);
            lastInstr_ = instr;
        }
    }

    void SetId(size_t id) {
        bbId_ = id;
    }

    size_t GetId() const {
        return bbId_;
    }

    void SetGraph(Graph* graph) {
        graph_ = graph;
    }

    void AddSuccessor(BasicBlock* block) {
        successors_.push_back(block);
    }

    void AddPredecessor(BasicBlock* block) {
        predecessors_.push_back(block);
    }

    Instruction* GetLastInstr() const {
        return lastInstr_;
    }

    void Dump(std::stringstream &ss) const;

private:
    size_t bbId_ {0};

    std::vector<BasicBlock*> predecessors_;
    std::vector<BasicBlock*> successors_;

    Instruction* firstPhi_ {nullptr};
    Instruction* firstInstr_ {nullptr};
    Instruction* lastInstr_ {nullptr};

    Graph* graph_ {nullptr};
};

#endif  // IR_BASIC_BLOCK_HPP