#include <gtest/gtest.h>

#include "DominatorTree/dominatortree.hpp"
#include "irbuilder.hpp"

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
TEST(DominatorTree, TEST_1) {
    Graph graph;
    IrBuilder builder(&graph);

    auto* a = builder.CreateBB();
    auto* b = builder.CreateBB();
    auto* c = builder.CreateBB();
    auto* d = builder.CreateBB();
    auto* e = builder.CreateBB();
    auto* f = builder.CreateBB();
    auto* g = builder.CreateBB();

    a->AddSuccessor(b);
    b->AddSuccessor(c);
    b->AddSuccessor(f);
    c->AddSuccessor(d);
    f->AddSuccessor(e);
    f->AddSuccessor(g);
    g->AddSuccessor(d);
    e->AddSuccessor(d);

    DominatorTree tree(&graph);
    tree.Build();

    auto &aImmDom = tree.GetImmediateDominatedBlocks(a);
    ASSERT_EQ(aImmDom.size(), 1);
    ASSERT_EQ(aImmDom[0], b);

    auto &bImmDom = tree.GetImmediateDominatedBlocks(b);
    ASSERT_EQ(bImmDom.size(), 3);

    std::set<BasicBlock*> expectedBImmDom = {c, f, d};
    for(auto it : bImmDom) {
        auto findIt = expectedBImmDom.find(it);
        ASSERT_FALSE(findIt == expectedBImmDom.end());
    }

    auto &cImmDom = tree.GetImmediateDominatedBlocks(c);
    ASSERT_EQ(cImmDom.size(), 0);

    auto &dImmDom = tree.GetImmediateDominatedBlocks(d);
    ASSERT_EQ(dImmDom.size(), 0);

    auto &fImmDom = tree.GetImmediateDominatedBlocks(f);
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
TEST(DominatorTree, TEST_2) {
    Graph graph;
    IrBuilder builder(&graph);

    auto* a = builder.CreateBB();
    auto* b = builder.CreateBB();
    auto* c = builder.CreateBB();
    auto* d = builder.CreateBB();
    auto* e = builder.CreateBB();
    auto* f = builder.CreateBB();
    auto* g = builder.CreateBB();
    auto* h = builder.CreateBB();
    auto* i = builder.CreateBB();
    auto* j = builder.CreateBB();
    auto* k = builder.CreateBB();

    a->AddSuccessor(b);
    b->AddSuccessor(c);
    b->AddSuccessor(j);
    j->AddSuccessor(c);
    c->AddSuccessor(d);
    d->AddSuccessor(c);
    d->AddSuccessor(e);
    e->AddSuccessor(f);
    f->AddSuccessor(e);
    f->AddSuccessor(g);
    g->AddSuccessor(i);
    g->AddSuccessor(h);
    h->AddSuccessor(b);
    i->AddSuccessor(k);

    DominatorTree tree(&graph);
    tree.Build();

    auto &aImmDom = tree.GetImmediateDominatedBlocks(a);
    ASSERT_EQ(aImmDom.size(), 1);
    ASSERT_EQ(aImmDom[0], b);

    auto &bImmDom = tree.GetImmediateDominatedBlocks(b);
    ASSERT_EQ(bImmDom.size(), 2);

    std::set<BasicBlock*> expectedBImmDom = {c, j};
    for(auto it : bImmDom) {
        auto findIt = expectedBImmDom.find(it);
        ASSERT_FALSE(findIt == expectedBImmDom.end());
    }

    auto &jImmDom = tree.GetImmediateDominatedBlocks(j);
    ASSERT_EQ(jImmDom.size(), 0);

    auto &cImmDom = tree.GetImmediateDominatedBlocks(c);
    ASSERT_EQ(cImmDom.size(), 1);
    ASSERT_EQ(cImmDom[0], d);

    auto &dImmDom = tree.GetImmediateDominatedBlocks(d);
    ASSERT_EQ(dImmDom.size(), 1);
    ASSERT_EQ(dImmDom[0], e);

    auto &eImmDom = tree.GetImmediateDominatedBlocks(e);
    ASSERT_EQ(eImmDom.size(), 1);
    ASSERT_EQ(eImmDom[0], f);

    auto &fImmDom = tree.GetImmediateDominatedBlocks(f);
    ASSERT_EQ(fImmDom.size(), 1);
    ASSERT_EQ(fImmDom[0], g);

    auto &gImmDom = tree.GetImmediateDominatedBlocks(g);
    ASSERT_EQ(gImmDom.size(), 2);

    std::set<BasicBlock*> expectedGImmDom = {h, i};
    for(auto it : gImmDom) {
        auto findIt = expectedGImmDom.find(it);
        ASSERT_FALSE(findIt == expectedGImmDom.end());
    }

    auto &iImmDom = tree.GetImmediateDominatedBlocks(i);
    ASSERT_EQ(iImmDom.size(), 1);
    ASSERT_EQ(iImmDom[0], k);

    auto &kImmDom = tree.GetImmediateDominatedBlocks(k);
    ASSERT_EQ(kImmDom.size(), 0);

    auto &hImmDom = tree.GetImmediateDominatedBlocks(h);
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
TEST(DominatorTree, TEST_3) {
    Graph graph;
    IrBuilder builder(&graph);

    auto* a = builder.CreateBB();
    auto* b = builder.CreateBB();
    auto* c = builder.CreateBB();
    auto* d = builder.CreateBB();
    auto* e = builder.CreateBB();
    auto* f = builder.CreateBB();
    auto* g = builder.CreateBB();
    auto* h = builder.CreateBB();
    auto* i = builder.CreateBB();

    a->AddSuccessor(b);

    b->AddSuccessor(c);
    b->AddSuccessor(e);

    c->AddSuccessor(d);

    e->AddSuccessor(d);
    e->AddSuccessor(f);

    d->AddSuccessor(g);

    f->AddSuccessor(b);
    f->AddSuccessor(h);

    h->AddSuccessor(i);
    h->AddSuccessor(g);

    g->AddSuccessor(i);
    g->AddSuccessor(c);

    DominatorTree tree(&graph);
    tree.Build();

    auto &aImmDom = tree.GetImmediateDominatedBlocks(a);
    ASSERT_EQ(aImmDom.size(), 1);
    ASSERT_EQ(aImmDom[0], b);

    auto &bImmDom = tree.GetImmediateDominatedBlocks(b);
    ASSERT_EQ(bImmDom.size(), 5);

    std::set<BasicBlock*> expectedBImmDom = {i, g, c, d, e};
    for(auto it : bImmDom) {
        auto findIt = expectedBImmDom.find(it);
        ASSERT_FALSE(findIt == expectedBImmDom.end());
    }

    auto &eImmDom = tree.GetImmediateDominatedBlocks(e);
    ASSERT_EQ(eImmDom.size(), 1);
    ASSERT_EQ(eImmDom[0], f);

    auto &fImmDom = tree.GetImmediateDominatedBlocks(f);
    ASSERT_EQ(fImmDom.size(), 1);
    ASSERT_EQ(fImmDom[0], h);

    auto &hImmDom = tree.GetImmediateDominatedBlocks(h);
    ASSERT_EQ(hImmDom.size(), 0);

    auto &iImmDom = tree.GetImmediateDominatedBlocks(i);
    ASSERT_EQ(iImmDom.size(), 0);

    auto &gImmDom = tree.GetImmediateDominatedBlocks(g);
    ASSERT_EQ(gImmDom.size(), 0);

    auto &cImmDom = tree.GetImmediateDominatedBlocks(c);
    ASSERT_EQ(cImmDom.size(), 0);

    auto &dImmDom = tree.GetImmediateDominatedBlocks(d);
    ASSERT_EQ(dImmDom.size(), 0);
}