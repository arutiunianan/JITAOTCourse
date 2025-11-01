#include "LoopAnalyzer/loopanalyzer.hpp"
#include "DFS/dfs.hpp"

#include <queue>
#include <algorithm>
#include <sstream>

void LoopAnalyzer::Analyze() {
    domTree_.Build();

    loops_.clear();
    headerToLoop_.clear();

    FindNaturalLoops();
    BuildLoopTree();
}

const std::vector<std::unique_ptr<Loop>>& LoopAnalyzer::GetLoops() const {
    return loops_;
}

void LoopAnalyzer::FindNaturalLoops() {
    DFS dfs(graph_);
    auto backEdges = dfs.RunLoopAnalyzer();

    for(auto &[header, tail]: backEdges) {
        if(!domTree_.Dominates(header, tail)) {
            continue;
        }

        Loop *loop = nullptr;
        auto it = headerToLoop_.find(header);
        if(it == headerToLoop_.end()) {
            loop = CreateNewLoop(header, tail);
        } else {
            loop = it->second;
            loop->AddBackEdge(tail);
        }

        std::queue<BasicBlock*> worklist;
        std::unordered_set<BasicBlock*> visited;

        worklist.push(tail);
        visited.insert(tail);
        loop->AddBlock(tail);

        while(!worklist.empty()) {
            BasicBlock *current = worklist.front();
            worklist.pop();

            for(BasicBlock *pred: current->GetPredecessors()) {
                if(pred != header && visited.find(pred) == visited.end()) {
                    worklist.push(pred);
                    visited.insert(pred);
                    loop->AddBlock(pred);
                }
            }
        }

        loop->AddBlock(header);
    }
}

Loop *LoopAnalyzer::CreateNewLoop(BasicBlock *header, BasicBlock *backEdge) {
    auto loop = std::make_unique<Loop>(header);
    Loop *loopPtr = loop.get();
    loop->AddBackEdge(backEdge);

    loops_.push_back(std::move(loop));
    headerToLoop_[header] = loopPtr;

    return loopPtr;
}

void LoopAnalyzer::BuildLoopTree() {
    std::sort(loops_.begin(), loops_.end(), 
        [](const std::unique_ptr<Loop> &a, const std::unique_ptr<Loop> &b) {
            return a->GetBlocks().size() > b->GetBlocks().size();
        });

    for(auto &loop: loops_) {
        for(auto &potentialParent: loops_) {
            if(potentialParent.get() == loop.get()) continue;

            if(potentialParent->Contains(loop->GetHeader())) {
                bool validParent = true;

                for(auto &otherParent: loops_) {
                    if(otherParent.get() == potentialParent.get() || 
                       otherParent.get() == loop.get()) continue;

                    if(potentialParent->Contains(otherParent->GetHeader()) && 
                       otherParent->Contains(loop->GetHeader())) {
                        validParent = false;
                        break;
                    }
                }

                if(validParent) {
                    loop->SetParent(potentialParent.get());
                    potentialParent->AddSubLoop(loop.get());
                    break;
                }
            }
        }
    }
}

void LoopAnalyzer::DumpLoops(std::ostream &ostr) const {
    ostr << "Loop Analyser Results:\n";
    for(const auto &loop: loops_) {
        ostr << "Header " << loop->GetHeader() << "\n";
        ostr << "  Blocks: ";
        for(BasicBlock* block: loop->GetBlocks()) {
            ostr << block << " ";
        }
        ostr << "\n";

        ostr << "  Back edges: ";
        for(BasicBlock* backEdge: loop->GetBackEdges()) {
            ostr << backEdge << "->" << loop->GetHeader() << " ";
        }
        ostr << "\n";

        if(loop->GetParent()) {
            ostr << "  Parent loop header: " << loop->GetParent()->GetHeader() << "\n";
        }

        if(!loop->GetSubLoops().empty()) {
            ostr << "  Sub-loops: ";
            for(Loop* subLoop: loop->GetSubLoops()) {
                ostr << subLoop->GetHeader() << " ";
            }
            ostr << "\n";
        }
        ostr << "-----------------\n";
    }

    if(loops_.empty()) {
        ostr << "No loops found:(\n";
    }
}

//--------------------------------------------------------------------

void Loop::AddBlock(BasicBlock *block) {
    blocks_.insert(block);
}

void Loop::AddBackEdge(BasicBlock *backEdge) {
    backEdges_.push_back(backEdge);
}

void Loop::SetParent(Loop *parent) {
    parent_ = parent;
}

void Loop::AddSubLoop(Loop *subLoop) {
    subLoops_.push_back(subLoop);
}

BasicBlock *Loop::GetHeader() const {
    return header_;
}

const std::unordered_set<BasicBlock*>& Loop::GetBlocks() const {
    return blocks_;
}

const std::vector<BasicBlock*>& Loop::GetBackEdges() const {
    return backEdges_;
}

Loop *Loop::GetParent() const {
    return parent_;
}

const std::vector<Loop*>& Loop::GetSubLoops() const {
    return subLoops_;
}

bool Loop::Contains(BasicBlock *block) const {
    return blocks_.count(block) > 0;
}