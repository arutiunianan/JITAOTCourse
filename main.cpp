#include "irbuilder.hpp"

#include <iostream>

int main() {
    Graph graph;
    IrBuilder builder(&graph);

    auto *entryBB = builder.CreateBB();
    auto *bb1 = builder.CreateBB();
    auto *bb2 = builder.CreateBB();
    auto *bb3 = builder.CreateBB();

    builder.SetBasicBlockScope(entryBB);
    auto *v0 = builder.CreateParameter(0);
    auto *v1 = builder.CreateInt64Constant(1);
    auto *v2 = builder.CreateInt64Constant(2);
    builder.CreateJmp(bb1);

    builder.SetBasicBlockScope(bb1);
    auto *v4 = builder.CreatePhi(DataType::U64);
    auto *v5 = builder.CreatePhi(DataType::U32);
    auto *v6 = builder.CreateCmp(v5, v0);
    builder.CreateJa(v6, bb3, bb2);

    builder.SetBasicBlockScope(bb2);
    auto *v8 = builder.CreateMul(DataType::U64, v4, v5);
    auto *v9 = builder.CreateAdd(DataType::U32, v5, v1);
    builder.CreateJmp(bb1);

    v4->AddInput(v1);
    v4->AddInput(v8);

    v5->AddInput(v2);
    v5->AddInput(v9);

    builder.SetBasicBlockScope(bb3);
    builder.CreateRet(DataType::U64, v4);

    std::stringstream ss;
    graph.Dump(ss);
    std::cout << ss.str() << std::endl;
    return 0;
}