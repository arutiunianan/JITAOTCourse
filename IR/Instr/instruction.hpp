#ifndef IR_INSTRUCTION_HPP
#define IR_INSTRUCTION_HPP

#include "Instr/enums.hpp"

#include <vector>
#include <sstream>

class Instruction;
class BasicBlock;

class Instruction {
public:
    Instruction(OpType optype, DataType resultType = DataType::UNDEFINED):
        optype_(optype), resultType_(resultType) {}

    virtual ~Instruction() = default;

    struct Input {
        Instruction* input = nullptr;
    };
    struct User {
        Instruction* user = nullptr;
    };

    void SetParentBB(BasicBlock* bb);
    BasicBlock* GetParentBB() const;

    void SetNext(Instruction* nextInstr);
    Instruction* GetNext() const;

    void SetPrev(Instruction* prevInstr);
    Instruction* GetPrev() const;

    void AddInput(Instruction* input);
    void AddUser(Instruction* user);

    void SetInputs(std::vector<Input> inputs);
    const std::vector<Input>& GetInputs() const;

    bool IsPhi() const;
    bool IsJmp() const;
    bool IsBranch() const;

    void SetResultType(DataType type);
    void SetId(size_t id);
    size_t GetId() const;

    virtual void Dump(std::stringstream &ss) const;

private:
    Instruction* prev_ = nullptr;
    Instruction* next_ = nullptr;

    BasicBlock* parentBB_ = nullptr;

    size_t instrId_ = 0;
    OpType optype_ = OpType::UNDEFINED;
    DataType resultType_;

    std::vector<Input> inputs_;
    std::vector<User> users_;
};

// ------------------------------------------------------------------------------------------------------

class ParameterInstr final: public Instruction {
public:
    ParameterInstr(uint32_t argNum): Instruction(OpType::PRM, DataType::U32), 
                                    argNum_(argNum) {}

    uint32_t GetArgNum() const;

    void Dump(std::stringstream &ss) const override;

private:
    uint32_t argNum_ = 0;
};

class ConstantInstr final: public Instruction {
public:
    template <typename T>
    ConstantInstr(T value): Instruction(OpType::CONST) {
        value_ = value;
        if constexpr (std::is_signed_v<T>) {
            type_ = DataType::I64;
        } else {
            type_ = DataType::U64;
        }
    }

    bool IsSignedInt() const;
    bool IsUnsignedInt() const;
    int64_t GetAsSignedInt() const;
    uint64_t GetAsUnsignedInt() const;

    void Dump(std::stringstream &ss) const override;

private:
    DataType type_ = DataType::UNDEFINED;

    uint64_t value_ = 0;
};

class PhiInstr final: public Instruction {
public:
    PhiInstr(DataType resultType): Instruction(OpType::PHI, resultType) {}

    BasicBlock* GetPhiInputBB(size_t idx);
    Instruction* GetPhiInput(BasicBlock* bb);

    void Dump(std::stringstream &ss) const override;
};

class ArithmeticInstr: public Instruction {
public:
    ArithmeticInstr(OpType opcode, DataType resultType,
                   Instruction* input1, Instruction* input2): 
        Instruction(opcode, resultType) {
        AddInput(input1);
        AddInput(input2);
        input1->AddUser(this);
        input2->AddUser(this);
    }

    void Dump(std::stringstream &ss) const override;
};

class AddInstr final: public ArithmeticInstr {
public:
    AddInstr(DataType resultType, Instruction* input1, Instruction* input2):
        ArithmeticInstr(OpType::ADD, resultType, input1, input2) {}
};

class SubInstr final: public ArithmeticInstr {
public:
    SubInstr(DataType resultType, Instruction* input1, Instruction* input2):
        ArithmeticInstr(OpType::SUB, resultType, input1, input2) {}
};

class MulInstr final: public ArithmeticInstr {
public:
    MulInstr(DataType resultType, Instruction* input1, Instruction* input2):
        ArithmeticInstr(OpType::MUL, resultType, input1, input2) {}
};

class DivInstr final: public ArithmeticInstr {
public:
    DivInstr(DataType resultType, Instruction* input1, Instruction* input2):
        ArithmeticInstr(OpType::DIV, resultType, input1, input2) {}
};

class AndInstr final: public ArithmeticInstr {
public:
    AndInstr(DataType resultType, Instruction* input1, Instruction* input2):
        ArithmeticInstr(OpType::ADD, resultType, input1, input2) {}
};

class JmpInstr final: public Instruction {
public:
    JmpInstr(BasicBlock* bbToJmp): Instruction(OpType::JMP, DataType::VOID), bbToJmp_(bbToJmp) {}

    BasicBlock* GetBBToJmp() const;

    void Dump(std::stringstream &ss) const override;

private:
    BasicBlock* bbToJmp_ {nullptr};
};

class CmpInstr final: public ArithmeticInstr {
public:
    CmpInstr(Instruction* input1, Instruction* input2):
        ArithmeticInstr(OpType::CMP, DataType::U8, input1, input2) {}
};

class CjmpInstr: public Instruction {
public:
    CjmpInstr(OpType optype, Instruction* input, BasicBlock* ifTrueBB, BasicBlock* ifFalseBB):
        Instruction(optype, DataType::VOID), ifTrueBB_(ifTrueBB), ifFalseBB_(ifFalseBB) {
        AddInput(input);
        input->AddUser(this);
    }

    BasicBlock* GetTrueBranchBB() const;
    BasicBlock* GetFalseBranchBB() const;

    void Dump(std::stringstream &ss) const override;

private:
    BasicBlock* ifTrueBB_ = nullptr;
    BasicBlock* ifFalseBB_ = nullptr;
};

class JaInstr final: public CjmpInstr {
public:
    JaInstr(Instruction* input, BasicBlock* ifTrueBB, BasicBlock* ifFalseBB):
        CjmpInstr(OpType::JA, input, ifTrueBB, ifFalseBB) {}
};

class JaeInstr final : public CjmpInstr {
public:
    JaeInstr(Instruction *input, BasicBlock *ifTrueBB, BasicBlock *ifFalseBB):
        CjmpInstr(OpType::JAE, input, ifTrueBB, ifFalseBB) {}
};

class JeInstr final : public CjmpInstr {
public:
    JeInstr(Instruction *input, BasicBlock *ifTrueBB, BasicBlock *ifFalseBB):
        CjmpInstr(OpType::JE, input, ifTrueBB, ifFalseBB) {}
};

class RetInstr final: public Instruction {
public:
    RetInstr(DataType retType, Instruction* input):
        Instruction(OpType::RET, retType), retValue_(input) {
        AddInput(input);
        input->AddUser(this);
    }

    void Dump(std::stringstream &ss) const override;

private:
    Instruction* retValue_ = nullptr;
};


#endif  // IR_INSTRUCTION_HPP