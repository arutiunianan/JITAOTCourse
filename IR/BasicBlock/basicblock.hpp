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
    void PushInstruction(Instruction* instr);

    void SetId(size_t id);
    size_t GetId() const;
    void SetGraph(Graph* graph);
    Instruction* GetLastInstr() const;

    void AddSuccessor(BasicBlock* block);
    void AddPredecessor(BasicBlock* block);

    void Dump(std::stringstream &ss) const;

private:
    size_t bbId_ = 0;

    std::vector<BasicBlock*> predecessors_;
    std::vector<BasicBlock*> successors_;

    // Instruction* firstPhi_ = nullptr;
    Instruction* firstInstr_ = nullptr;
    Instruction* lastInstr_ = nullptr;

    Graph* graph_ = nullptr;
};

#endif  // IR_BASIC_BLOCK_HPP