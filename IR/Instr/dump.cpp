#include "Instr/instruction.hpp"
#include "Instr/enums.hpp"
#include "BasicBlock/basicblock.hpp"

std::string OpToString(OpType optype) {
    switch(optype)
    {
        #define OPR_DEF(name, dump_name) \
        case OpType::name:                \
            return dump_name;              \
            break;                                 

        #include "oprdef.hpp"
        #undef REG_DEF
    }
}

std::string DataTypeToStr(DataType datatype) {
    switch(datatype)
    {
        #define DATA_DEF(name, dump_name) \
        case DataType::name:               \
            return dump_name;               \
            break;                                 

        #include "datadef.hpp"
        #undef REG_DEF
    }
}

void Instruction::Dump(std::stringstream &ss) const
{
    ss << instrId_ << ". " << DataTypeToStr(resultType_) << " " << OpToString(optype_) << " ";
}

void ParameterInstr::Dump(std::stringstream &ss) const
{
    Instruction::Dump(ss);
    ss << argNum_;
}

void ConstantInstr::Dump(std::stringstream &ss) const
{
    Instruction::Dump(ss);
    if (IsSignedInt()) {
        ss << GetAsSignedInt();
    } else if (IsUnsignedInt()) {
        ss << GetAsUnsignedInt();
    } else {
        ss << "gadaaaamn(";
    }
}

void PhiInstr::Dump(std::stringstream &ss) const
{
    Instruction::Dump(ss);
    auto &inputs = GetInputs();

    for (size_t idx = 0; idx < inputs.size(); ++idx) {
        ss << "v" << inputs[idx].input->GetId() << ":BB_" << inputs[idx].input->GetParentBB()->GetId();
        if (idx != inputs.size() - 1) {
            ss << ", ";
        }
    }
}

void ArithmeticInstr::Dump(std::stringstream &ss) const
{
    Instruction::Dump(ss);

    auto &inputs = GetInputs();

    for (size_t idx = 0; idx < inputs.size(); ++idx) {
        ss << "v" << inputs[idx].input->GetId();
        if (idx != inputs.size() - 1) {
            ss << ", ";
        }
    }
}

void JmpInstr::Dump(std::stringstream &ss) const
{
    Instruction::Dump(ss);
    ss << "BB_" << GetBBToJmp()->GetId();
}

void CjmpInstr::Dump(std::stringstream &ss) const
{
    Instruction::Dump(ss);
    auto &inputs = GetInputs();
    for (auto &i : inputs) {
        ss << "v" << i.input->GetId() << ", ";
    }

    ss << "BB_" << GetTrueBranchBB()->GetId() << ", BB_" << GetFalseBranchBB()->GetId();
}

void RetInstr::Dump(std::stringstream &ss) const
{
    Instruction::Dump(ss);
    ss << "v" << retValue_->GetId();
}