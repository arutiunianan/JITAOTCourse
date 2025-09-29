#include "BasicBlock/basicblock.hpp"

#include <iomanip>

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