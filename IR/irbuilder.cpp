#include "irbuilder.hpp"

Instruction* IrBuilder::CreatePhi(DataType resultType) {
    return CreateInstruction<PhiInstr>(resultType);
}

Instruction* IrBuilder::CreateParameter(uint32_t parameter) {
    return CreateInstruction<ParameterInstr>(parameter);
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