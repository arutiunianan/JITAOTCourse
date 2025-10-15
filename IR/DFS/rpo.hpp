#ifndef IR_RPO_HPP
#define IR_RPO_HPP

#include "DFS/dfs.hpp"

#include <vector>

class RPO final {
public:
    RPO(Graph* graph): graph_(graph) {}

    std::vector<BasicBlock*> Run() {
        DFS dfs{graph_};
        std::vector<BasicBlock*> rpoVector = dfs.Run();
        std::reverse(rpoVector.begin(), rpoVector.end());
        return rpoVector;
    }

private:
    Graph* graph_ = nullptr;
};

#endif  // IR_RPO_HPP