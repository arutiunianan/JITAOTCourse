#ifndef IR_BUILDER_HPP
#define IR_BUILDER_HPP

#include "Instr/instruction.hpp"
#include "BasicBlock/basicblock.hpp"
#include "Graph/graph.hpp"

#include <string>
#include <vector>
#include <memory>

class IrBuilder final {
public:
    IrBuilder(Graph* graph): graph_(graph) {}

    BasicBlock* CreateBB() {
        auto bb = std::make_unique<BasicBlock>();
        auto bbPtr = bb.get();
        graph_->AddBlock(std::move(bb));
        return bbPtr;
    }

    void SetBasicBlockScope(BasicBlock* currentBB) {
        currentBB_ = currentBB;
    }

    template <typename InstT, typename... ArgsT>
    Instruction* CreateInstruction(ArgsT &&...args);

    Instruction* CreatePhi(DataType resultType);

    Instruction* CreateParameter(uint32_t parameter);

    template <typename T>
    Instruction* CreateConstant(T constant, DataType resultType);
    Instruction* CreateInt32Constant(uint32_t constant);
    Instruction* CreateInt64Constant(uint64_t constant);

    Instruction* CreateAdd(DataType resultType, Instruction* input1, Instruction* input2);
    Instruction* CreateSub(DataType resultType, Instruction* input1, Instruction* input2);
    Instruction* CreateMul(DataType resultType, Instruction* input1, Instruction* input2);
    Instruction* CreateDiv(DataType resultType, Instruction* input1, Instruction* input2);

    Instruction* CreateJmp(BasicBlock* bbToJmp);
    Instruction* CreateCmp(Instruction* input1, Instruction* input2);
    Instruction* CreateJa(Instruction* input, BasicBlock* bb1, BasicBlock* bb2);
    Instruction* CreateJae(Instruction* input, BasicBlock* bb1, BasicBlock* bb2);
    Instruction* CreateJe(Instruction* input, BasicBlock* bb1, BasicBlock* bb2);

    Instruction* CreateRet(DataType retType, Instruction* input);

private:
    Graph* graph_ {nullptr};

    BasicBlock* currentBB_ {nullptr};
};


template <typename InstT, typename... ArgsT>
Instruction* IrBuilder::CreateInstruction(ArgsT &&...args) {
    auto instr = std::make_unique<InstT>(std::forward<ArgsT>(args)...);
    Instruction* instrPtr = instr.get();
    graph_->AddInstruction(std::move(instr));

    instrPtr->SetParentBB(currentBB_);
    currentBB_->PushInstruction(instrPtr);

    if (instrPtr->IsBranch()) {
        auto* branchInst = static_cast<CjmpInstr* >(instrPtr);
        auto* ifTrueBB = branchInst->GetTrueBranchBB();
        auto* ifFalseBB = branchInst->GetFalseBranchBB();

        currentBB_->AddSuccessor(ifTrueBB);
        currentBB_->AddSuccessor(ifFalseBB);

        ifTrueBB->AddPredecessor(currentBB_);
        ifFalseBB->AddPredecessor(currentBB_);
    }

    if (instrPtr->IsJmp()) {
        auto* jmpInst = static_cast<JmpInstr* >(instrPtr);
        auto* bbToJmp = jmpInst->GetBBToJmp();

        currentBB_->AddSuccessor(bbToJmp);
        bbToJmp->AddPredecessor(currentBB_);
    }

    return instrPtr;
}

Instruction* IrBuilder::CreatePhi(DataType resultType) {
    return CreateInstruction<PhiInstr>(resultType);
}

Instruction* IrBuilder::CreateParameter(uint32_t parameter) {
    return CreateInstruction<ParameterInstr>(parameter);
}

template <typename T>
Instruction* IrBuilder::CreateConstant(T constant, DataType resultType) {
    auto instr = CreateInstruction<ConstantInstr>(constant);
    instr->SetResultType(resultType);
    return instr;
}

Instruction* IrBuilder::CreateInt32Constant(uint32_t constant) {
    return CreateConstant(constant, DataType::I32);
}

Instruction* IrBuilder::CreateInt64Constant(uint64_t constant) {
    return CreateConstant(constant, DataType::I64);
}

Instruction* IrBuilder::CreateAdd(DataType resultType, Instruction* input1, Instruction* input2) {
    return CreateInstruction<AddInstr>(resultType, input1, input2);
}

Instruction* IrBuilder::CreateSub(DataType resultType, Instruction* input1, Instruction* input2) {
    return CreateInstruction<SubInstr>(resultType, input1, input2);
}

Instruction* IrBuilder::CreateMul(DataType resultType, Instruction* input1, Instruction* input2) {
    return CreateInstruction<MulInstr>(resultType, input1, input2);
}

Instruction* IrBuilder::CreateDiv(DataType resultType, Instruction* input1, Instruction* input2) {
    return CreateInstruction<DivInstr>(resultType, input1, input2);
}

Instruction* IrBuilder::CreateJmp(BasicBlock* bbToJmp) {
    return CreateInstruction<JmpInstr>(bbToJmp);
}

Instruction* IrBuilder::CreateCmp(Instruction* input1, Instruction* input2) {
    return CreateInstruction<CmpInstr>(input1, input2);
}

Instruction* IrBuilder::CreateJa(Instruction* input, BasicBlock* bb1, BasicBlock* bb2) {
    return CreateInstruction<JaInstr>(input, bb1, bb2);
}

Instruction* IrBuilder::CreateJae(Instruction* input, BasicBlock* bb1, BasicBlock* bb2) {
    return CreateInstruction<JaeInstr>(input, bb1, bb2);
}

Instruction* IrBuilder::CreateJe(Instruction* input, BasicBlock* bb1, BasicBlock* bb2) {
    return CreateInstruction<JeInstr>(input, bb1, bb2);
}

Instruction* IrBuilder::CreateRet(DataType retType, Instruction* input) {
    return CreateInstruction<RetInstr>(retType, input);
}

#endif  // IR_BUILDER_HPP