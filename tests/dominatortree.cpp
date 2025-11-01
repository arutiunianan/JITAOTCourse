#include <gtest/gtest.h>

#include "DominatorTree/dominatortree.hpp"
#include "irbuilder.hpp"

class DominatorTreeTest: public ::testing::Test {
protected:
    void SetUp() override {
        graph_ = std::make_unique<Graph>();
    }

    void TearDown() override {
        graph_.reset();
    }

    BasicBlock *CreateBlock() {
        auto block = std::make_unique<BasicBlock>();
        BasicBlock *ptr = block.get();
        graph_->AddBlock(std::move(block));
        return ptr;
    }

    void LinkBlocks(BasicBlock *from, BasicBlock *to) {
        from->AddSuccessor(to);
        to->AddPredecessor(from);
    }

    void BuildDominatorTree() {
        tree_ = std::make_unique<DominatorTree>(graph_.get());
        tree_->Build();
    }

    std::unique_ptr<Graph> graph_;
    std::unique_ptr<DominatorTree> tree_;
};

/*
    Graph:    Dominator Tree:
      A             A
      |             |
      B             B
    /   \         / | \
   C     F       C  F  D
   \    / \        / \
    \  E   G      E   G
     \ |  /
       D
*/
TEST_F(DominatorTreeTest, TEST_1) {
    Graph graph;

    auto *a = CreateBlock();
    auto *b = CreateBlock();
    auto *c = CreateBlock();
    auto *d = CreateBlock();
    auto *e = CreateBlock();
    auto *f = CreateBlock();
    auto *g = CreateBlock();

    LinkBlocks(a, b);
    LinkBlocks(b, c);
    LinkBlocks(b, f);
    LinkBlocks(c, d);
    LinkBlocks(f, e);
    LinkBlocks(f, g);
    LinkBlocks(g, d);
    LinkBlocks(e, d);

    BuildDominatorTree();

    auto &aImmDom = tree_->GetImmediateDominatedBlocks(a);
    ASSERT_EQ(aImmDom.size(), 1);
    ASSERT_EQ(aImmDom[0], b);

    auto &bImmDom = tree_->GetImmediateDominatedBlocks(b);
    ASSERT_EQ(bImmDom.size(), 3);

    std::set<BasicBlock*> expectedBImmDom = {c, f, d};
    for(auto it : bImmDom) {
        auto findIt = expectedBImmDom.find(it);
        ASSERT_FALSE(findIt == expectedBImmDom.end());
    }

    auto &cImmDom = tree_->GetImmediateDominatedBlocks(c);
    ASSERT_EQ(cImmDom.size(), 0);

    auto &dImmDom = tree_->GetImmediateDominatedBlocks(d);
    ASSERT_EQ(dImmDom.size(), 0);

    auto &fImmDom = tree_->GetImmediateDominatedBlocks(f);
    ASSERT_EQ(fImmDom.size(), 2);

    std::set<BasicBlock*> expectedFImmDom = {e, g};
    for(auto it : fImmDom) {
        auto findIt = expectedFImmDom.find(it);
        ASSERT_FALSE(findIt == expectedFImmDom.end());
    }
}

/*
    Graph:
          J ----\
          ^     |
          |     v
    A --> B --> C --> D --> E --> F --> G --> I --> K
          ^     ^     |     ^     |     |
          |     |     |     |     |     |
          |     \-----/     \-----/     |
          |                             v
          \---------------------------- H

    Dominator tree:

    A --> B --> C --> D --> E --> F --> G --> I --> K
          |                             |
          v                             v
          J                             H
*/
TEST_F(DominatorTreeTest, TEST_2) {
    Graph graph;
    IrBuilder builder(&graph);

    auto *a = CreateBlock();
    auto *b = CreateBlock();
    auto *c = CreateBlock();
    auto *d = CreateBlock();
    auto *e = CreateBlock();
    auto *f = CreateBlock();
    auto *g = CreateBlock();
    auto *h = CreateBlock();
    auto *i = CreateBlock();
    auto *j = CreateBlock();
    auto *k = CreateBlock();

    LinkBlocks(a, b);
    LinkBlocks(b, c);
    LinkBlocks(b, j);
    LinkBlocks(j, c);
    LinkBlocks(c, d);
    LinkBlocks(d, c);
    LinkBlocks(d, e);
    LinkBlocks(e, f);
    LinkBlocks(f, e);
    LinkBlocks(f, g);
    LinkBlocks(g, i);
    LinkBlocks(g, h);
    LinkBlocks(h, b);
    LinkBlocks(i, k);

    BuildDominatorTree();

    auto &aImmDom = tree_->GetImmediateDominatedBlocks(a);
    ASSERT_EQ(aImmDom.size(), 1);
    ASSERT_EQ(aImmDom[0], b);

    auto &bImmDom = tree_->GetImmediateDominatedBlocks(b);
    ASSERT_EQ(bImmDom.size(), 2);

    std::set<BasicBlock*> expectedBImmDom = {c, j};
    for(auto it : bImmDom) {
        auto findIt = expectedBImmDom.find(it);
        ASSERT_FALSE(findIt == expectedBImmDom.end());
    }

    auto &jImmDom = tree_->GetImmediateDominatedBlocks(j);
    ASSERT_EQ(jImmDom.size(), 0);

    auto &cImmDom = tree_->GetImmediateDominatedBlocks(c);
    ASSERT_EQ(cImmDom.size(), 1);
    ASSERT_EQ(cImmDom[0], d);

    auto &dImmDom = tree_->GetImmediateDominatedBlocks(d);
    ASSERT_EQ(dImmDom.size(), 1);
    ASSERT_EQ(dImmDom[0], e);

    auto &eImmDom = tree_->GetImmediateDominatedBlocks(e);
    ASSERT_EQ(eImmDom.size(), 1);
    ASSERT_EQ(eImmDom[0], f);

    auto &fImmDom = tree_->GetImmediateDominatedBlocks(f);
    ASSERT_EQ(fImmDom.size(), 1);
    ASSERT_EQ(fImmDom[0], g);

    auto &gImmDom = tree_->GetImmediateDominatedBlocks(g);
    ASSERT_EQ(gImmDom.size(), 2);

    std::set<BasicBlock*> expectedGImmDom = {h, i};
    for(auto it : gImmDom) {
        auto findIt = expectedGImmDom.find(it);
        ASSERT_FALSE(findIt == expectedGImmDom.end());
    }

    auto &iImmDom = tree_->GetImmediateDominatedBlocks(i);
    ASSERT_EQ(iImmDom.size(), 1);
    ASSERT_EQ(iImmDom[0], k);

    auto &kImmDom = tree_->GetImmediateDominatedBlocks(k);
    ASSERT_EQ(kImmDom.size(), 0);

    auto &hImmDom = tree_->GetImmediateDominatedBlocks(h);
    ASSERT_EQ(hImmDom.size(), 0);
}

/*
    Graph:
            A      /--------------------\
            |      |                    |
            v      v                    |
      /---> B ---> C ---> D ----------> G ---> I
      |     |             ^             ^      ^
      |     |             |             |      |
      |     \-----------> E ---> F ---> H -----/
      |                          |
      \--------------------------/

    Dominator tree:

             /---> D
            /---> C
    A ---> B ---> E ---> F ---> H
            \---> G
             \---> I
*/
TEST_F(DominatorTreeTest, TEST_3) {
    Graph graph;
    IrBuilder builder(&graph);

    auto *a = CreateBlock();
    auto *b = CreateBlock();
    auto *c = CreateBlock();
    auto *d = CreateBlock();
    auto *e = CreateBlock();
    auto *f = CreateBlock();
    auto *g = CreateBlock();
    auto *h = CreateBlock();
    auto *i = CreateBlock();

    LinkBlocks(a, b);

    LinkBlocks(b, c);
    LinkBlocks(b, e);

    LinkBlocks(c, d);

    LinkBlocks(e, d);
    LinkBlocks(e, f);

    LinkBlocks(d, g);

    LinkBlocks(f, b);
    LinkBlocks(f, h);

    LinkBlocks(h, i);
    LinkBlocks(h, g);

    LinkBlocks(g, i);
    LinkBlocks(g, c);

    BuildDominatorTree();

    auto &aImmDom = tree_->GetImmediateDominatedBlocks(a);
    ASSERT_EQ(aImmDom.size(), 1);
    ASSERT_EQ(aImmDom[0], b);

    auto &bImmDom = tree_->GetImmediateDominatedBlocks(b);
    ASSERT_EQ(bImmDom.size(), 5);

    std::set<BasicBlock*> expectedBImmDom = {i, g, c, d, e};
    for(auto it : bImmDom) {
        auto findIt = expectedBImmDom.find(it);
        ASSERT_FALSE(findIt == expectedBImmDom.end());
    }

    auto &eImmDom = tree_->GetImmediateDominatedBlocks(e);
    ASSERT_EQ(eImmDom.size(), 1);
    ASSERT_EQ(eImmDom[0], f);

    auto &fImmDom = tree_->GetImmediateDominatedBlocks(f);
    ASSERT_EQ(fImmDom.size(), 1);
    ASSERT_EQ(fImmDom[0], h);

    auto &hImmDom = tree_->GetImmediateDominatedBlocks(h);
    ASSERT_EQ(hImmDom.size(), 0);

    auto &iImmDom = tree_->GetImmediateDominatedBlocks(i);
    ASSERT_EQ(iImmDom.size(), 0);

    auto &gImmDom = tree_->GetImmediateDominatedBlocks(g);
    ASSERT_EQ(gImmDom.size(), 0);

    auto &cImmDom = tree_->GetImmediateDominatedBlocks(c);
    ASSERT_EQ(cImmDom.size(), 0);

    auto &dImmDom = tree_->GetImmediateDominatedBlocks(d);
    ASSERT_EQ(dImmDom.size(), 0);
}