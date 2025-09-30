#include "Instr/instruction.hpp"

void Instruction::SetParentBB(BasicBlock* bb) {
    parentBB_ = bb;
}

BasicBlock* Instruction::GetParentBB() const {
    return parentBB_;
}

void Instruction::SetNext(Instruction* nextInstr) {
    next_ = nextInstr;
}

Instruction* Instruction::GetNext() const {
    return next_;
}

void Instruction::SetPrev(Instruction* prevInstr) {
    prev_ = prevInstr;
}

Instruction* Instruction::GetPrev() const {
    return prev_;
}

void Instruction::AddInput(Instruction* input) {
    inputs_.push_back(Input {input});
}

void Instruction::AddUser(Instruction* user) {
    users_.push_back(User {user});
}

void Instruction::SetInputs(std::vector<Input> inputs) {
    inputs_ = std::move(inputs);
}

const std::vector<Instruction::Input>& Instruction::GetInputs() const {
    return inputs_;
}

bool Instruction::IsPhi() const {
    return optype_ == OpType::PHI;
}

bool Instruction::IsJmp() const {
    return optype_ == OpType::JMP;
}

bool Instruction::IsBranch() const {
    return (optype_ == OpType::JA)  ||
           (optype_ == OpType::JAE) ||
           (optype_ == OpType::JE);
}

void Instruction::SetResultType(DataType type) {
    resultType_ = type;
}

void Instruction::SetId(size_t id) {
    instrId_ = id;
}

size_t Instruction::GetId() const {
    return instrId_;
}

// ------------------------------------------------------------------------------------------------------

uint32_t ParameterInstr::GetArgNum() const {
    return argNum_;
}


bool ConstantInstr::IsSignedInt() const {
    return type_ == DataType::I64;
}

bool ConstantInstr::IsUnsignedInt() const {
    return type_ == DataType::U64;
}

int64_t ConstantInstr::GetAsSignedInt() const {
    return static_cast<int64_t>(value_);
}

uint64_t ConstantInstr::GetAsUnsignedInt() const {
    return value_;
}


BasicBlock* JmpInstr::GetBBToJmp() const {
    return bbToJmp_;
}


BasicBlock* CjmpInstr::GetTrueBranchBB() const {
    return ifTrueBB_;
}

BasicBlock* CjmpInstr::GetFalseBranchBB() const {
    return ifFalseBB_;
}