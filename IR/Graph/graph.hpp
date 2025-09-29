#ifndef IR_GRAPH_HPP
#define IR_GRAPH_HPP

#include <memory>
#include <vector>

#include "BasicBlock/basicblock.hpp"
#include "Instr/instruction.hpp"

class Graph final {
public:
    void AddBlock(std::unique_ptr<BasicBlock> block);
    void AddInstruction(std::unique_ptr<Instruction> instr);

    void Dump(std::stringstream &ss) const;

private:
    std::vector<std::unique_ptr<BasicBlock>> basicBlocks_;
    std::vector<std::unique_ptr<Instruction>> instructions_;
};

#endif  // IR_GRAPH_HPP