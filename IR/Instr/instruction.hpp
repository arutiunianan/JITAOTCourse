#ifndef IR_INSTRUCTION_HPP
#define IR_INSTRUCTION_HPP

#include "Instr/enums.hpp"

#include <vector>
#include <sstream>

class Instruction;
class BasicBlock;

struct Input {
    Instruction* input {nullptr};
};

struct User {
    Instruction* user {nullptr};
};

class Instruction {
public:
    Instruction(OpType optype, DataType resultType = DataType::UNDEFINED):
        optype_(optype), resultType_(resultType) {}

    virtual ~Instruction() = default;

    void SetParentBB(BasicBlock* bb) {
        parentBB_ = bb;
    }

    BasicBlock* GetParentBB() const {
        return parentBB_;
    }

    void SetNext(Instruction* nextInstr) {
        next_ = nextInstr;
    }

    Instruction* GetNext() const {
        return next_;
    }

    void SetPrev(Instruction* prevInstr) {
        prev_ = prevInstr;
    }

    Instruction* GetPrev() const {
        return prev_;
    }

    void AddInput(Instruction* input) {
        inputs_.push_back(Input {input});
    }

    void AddUser(Instruction* user) {
        users_.push_back(User {user});
    }

    void SetInputs(std::vector<Input> inputs) {
        inputs_ = std::move(inputs);
    }

    const std::vector<Input> &GetInputs() const {
        return inputs_;
    }

    bool IsPhi() const {
        return optype_ == OpType::PHI;
    }

    bool IsJmp() const {
        return optype_ == OpType::JMP;
    }

    bool IsBranch() const {
        return (optype_ == OpType::JA)  ||
               (optype_ == OpType::JAE) ||
               (optype_ == OpType::JE);
    }

    void SetResultType(DataType type) {
        resultType_ = type;
    }

    void SetId(size_t id) {
        instrId_ = id;
    }

    size_t GetId() const {
        return instrId_;
    }

    virtual void Dump(std::stringstream &ss) const;

private:
    Instruction* prev_ {nullptr};
    Instruction* next_ {nullptr};

    BasicBlock* parentBB_ {nullptr};

    size_t instrId_ {0};
    OpType optype_ {OpType::UNDEFINED};
    DataType resultType_;

    std::vector<Input> inputs_;
    std::vector<User> users_;
};

// ------------------------------------------------------------------------------------------------------

class ParameterInstr final: public Instruction {
public:
    ParameterInstr(uint32_t argNum): Instruction(OpType::PRM, DataType::U32), 
                                    argNum_(argNum) {}

    uint32_t GetArgNum() const {
        return argNum_;
    }

    void Dump(std::stringstream &ss) const override;

private:
    uint32_t argNum_ {0};
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

    bool IsSignedInt() const {
        return type_ == DataType::I64;
    }

    bool IsUnsignedInt() const {
        return type_ == DataType::U64;
    }

    int64_t GetAsSignedInt() const {
        return static_cast<int64_t>(value_);
    }

    uint64_t GetAsUnsignedInt() const {
        return value_;
    }

    void Dump(std::stringstream &ss) const override;

private:
    DataType type_ {DataType::UNDEFINED};

    uint64_t value_ {0};
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

    BasicBlock* GetBBToJmp() const {
        return bbToJmp_;
    }

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

    BasicBlock* GetTrueBranchBB() const {
        return ifTrueBB_;
    }

    BasicBlock* GetFalseBranchBB() const {
        return ifFalseBB_;
    }

    void Dump(std::stringstream &ss) const override;

private:
    BasicBlock* ifTrueBB_ {nullptr};
    BasicBlock* ifFalseBB_ {nullptr};
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
    Instruction* retValue_ {nullptr};
};


#endif  // IR_INSTRUCTION_HPP