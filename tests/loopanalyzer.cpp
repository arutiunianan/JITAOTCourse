#include <gtest/gtest.h>

#include "LoopAnalyzer/loopanalyzer.hpp"
#include "Graph/graph.hpp"

class LoopAnalyzerTest: public ::testing::Test {
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

    void AnalyzeLoops() {
        analyzer_ = std::make_unique<LoopAnalyzer>(graph_.get());
        analyzer_->Analyze();
    }

    std::unique_ptr<Graph> graph_;
    std::unique_ptr<LoopAnalyzer> analyzer_;
};

/*
    Graph:

        +-----------+
        V           |
        A-->B-->D-->E
            |
            V
            C
*/
TEST_F(LoopAnalyzerTest, TEST_1) {
    auto *a = CreateBlock();
    auto *b = CreateBlock();
    auto *c = CreateBlock();
    auto *d = CreateBlock();
    auto *e = CreateBlock();

    LinkBlocks(a, b);
    LinkBlocks(b, d);
    LinkBlocks(b, c);
    LinkBlocks(d, e);
    LinkBlocks(e, a);

    AnalyzeLoops();
    const auto &loops = analyzer_->GetLoops();

    ASSERT_EQ(loops.size(), 1);

    const auto &loop = loops[0];
    EXPECT_EQ(loop->GetHeader(), a);

    const auto &loopBlocks = loop->GetBlocks();
    EXPECT_EQ(loopBlocks.size(), 4);
    EXPECT_TRUE(loopBlocks.count(a));
    EXPECT_TRUE(loopBlocks.count(b));
    EXPECT_TRUE(loopBlocks.count(d));
    EXPECT_TRUE(loopBlocks.count(e));

    const auto &backEdges = loop->GetBackEdges();
    ASSERT_EQ(backEdges.size(), 1);
    EXPECT_EQ(backEdges[0], e);

    EXPECT_EQ(loop->GetParent(), nullptr);
    EXPECT_TRUE(loop->GetSubLoops().empty());
}

/*
    Graph:

    A--->B<-----------+
         |            |
         V            |
         C------>D--->E
         |       |
         +-->F<--+

*/
TEST_F(LoopAnalyzerTest, TEST_2) {
    auto *a = CreateBlock();
    auto *b = CreateBlock();
    auto *c = CreateBlock();
    auto *d = CreateBlock();
    auto *e = CreateBlock();
    auto *f = CreateBlock();

    LinkBlocks(a, b);
    LinkBlocks(b, c);
    LinkBlocks(c, d);
    LinkBlocks(c, f);
    LinkBlocks(d, e);
    LinkBlocks(d, f);
    LinkBlocks(e, b);

    AnalyzeLoops();
    const auto &loops = analyzer_->GetLoops();

    ASSERT_EQ(loops.size(), 1);

    const auto &loop = loops[0];
    EXPECT_EQ(loop->GetHeader(), b);

    const auto &loopBlocks = loop->GetBlocks();
    EXPECT_EQ(loopBlocks.size(), 4);
    EXPECT_TRUE(loopBlocks.count(b));
    EXPECT_TRUE(loopBlocks.count(c));
    EXPECT_TRUE(loopBlocks.count(d));
    EXPECT_TRUE(loopBlocks.count(e));

    const auto &backEdges = loop->GetBackEdges();
    ASSERT_EQ(backEdges.size(), 1);
    EXPECT_EQ(backEdges[0], e);

    EXPECT_EQ(loop->GetParent(), nullptr);
    EXPECT_TRUE(loop->GetSubLoops().empty());
}

/*
    Graph:

    +------------------------+
    |    +--------------+    |
    |    |              |    |
    v    v              |    |
    A--->B--->C--->D--->E--->F
         |    ^
         v    |
         G----+
         |
         v
         H
*/
TEST_F(LoopAnalyzerTest, TEST_3) {
    auto *a = CreateBlock();
    auto *b = CreateBlock();
    auto *c = CreateBlock();
    auto *d = CreateBlock();
    auto *e = CreateBlock();
    auto *f = CreateBlock();
    auto *g = CreateBlock();
    auto *h = CreateBlock();

    LinkBlocks(a, b);
    LinkBlocks(b, c);
    LinkBlocks(b, g);
    LinkBlocks(c, d);
    LinkBlocks(d, e);
    LinkBlocks(e, b);
    LinkBlocks(e, f);
    LinkBlocks(g, h);
    LinkBlocks(g, c);
    LinkBlocks(f, a);

    AnalyzeLoops();
    const auto &loops = analyzer_->GetLoops();

    ASSERT_GE(loops.size(), 1);

    const Loop *outerLoop = nullptr;
    for(const auto &loop: loops) {
        if(loop->GetHeader() == a) {
            outerLoop = loop.get();
            break;
        }
    }
    ASSERT_NE(outerLoop, nullptr);

    const auto &outerBlocks = outerLoop->GetBlocks();
    EXPECT_TRUE(outerBlocks.count(a));
    EXPECT_TRUE(outerBlocks.count(f));

    const auto &outerBackEdges = outerLoop->GetBackEdges();
    ASSERT_GE(outerBackEdges.size(), 1);
    EXPECT_EQ(outerBackEdges[0], f);

    ASSERT_EQ(outerLoop->GetSubLoops().size(), 1);

    const Loop *innerLoop = outerLoop->GetSubLoops()[0];
    EXPECT_EQ(innerLoop->GetHeader(), b);

    const auto &innerBlocks = innerLoop->GetBlocks();
    EXPECT_TRUE(innerBlocks.count(b));
    EXPECT_TRUE(innerBlocks.count(c));
    EXPECT_TRUE(innerBlocks.count(d));
    EXPECT_TRUE(innerBlocks.count(e));
    EXPECT_TRUE(innerBlocks.count(g));

    const auto &innerBackEdges = innerLoop->GetBackEdges();
    ASSERT_GE(innerBackEdges.size(), 1);
    EXPECT_EQ(innerBackEdges[0], e);
}

/*
    Graph:

    A--->B--->F--->G
         |    |    |
         |    v    |
         |    E    |
         |    |    |
         v    v    |
         C--->D<---+

*/
TEST_F(LoopAnalyzerTest, TEST_4) {
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
    LinkBlocks(e, d);
    LinkBlocks(f, e);
    LinkBlocks(f, g);
    LinkBlocks(g, d);

    AnalyzeLoops();
    const auto &loops = analyzer_->GetLoops();

    EXPECT_TRUE(loops.empty());
}

/*
    Graph:

          J-----+
          ^     |
          |     v
    A---->B---->C---->D---->E---->F---->G---->I---->K
          ^     ^     |     ^     |     |
          |     |     |     |     |     |
          |     +-----+     +-----+     |
          |                             v
          +-----------------------------H
*/
TEST_F(LoopAnalyzerTest, TEST_5) {
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
    LinkBlocks(c, d);
    LinkBlocks(d, c);
    LinkBlocks(d, e);
    LinkBlocks(e, f);
    LinkBlocks(f, e);
    LinkBlocks(f, g);
    LinkBlocks(g, h);
    LinkBlocks(g, i);
    LinkBlocks(h, b);
    LinkBlocks(i, k);
    LinkBlocks(j, c);

    AnalyzeLoops();
    const auto &loops = analyzer_->GetLoops();

    ASSERT_GE(loops.size(), 1);

    const Loop *mainLoop = nullptr;
    for(const auto &loop: loops) {
        if(loop->GetHeader() == b) {
            mainLoop = loop.get();
            break;
        }
    }
    ASSERT_NE(mainLoop, nullptr);

    ASSERT_GE(mainLoop->GetSubLoops().size(), 1);

    bool foundLoopC = false;
    bool foundLoopE = false;

    for(const auto &subLoop: mainLoop->GetSubLoops()) {
        if(subLoop->GetHeader() == c) {
            foundLoopC = true;
        } else if(subLoop->GetHeader() == e) {
            foundLoopE = true;
        }
    }

    EXPECT_TRUE(foundLoopC);
    EXPECT_TRUE(foundLoopE);
}

/*
    Graph:
            A      +--------------------+
            |      |                    |
            v      v                    |
      +---->B----->C----->D------------>G----->I
      |     |             ^             ^      ^
      |     |             |             |      |
      |     +------------>E----->F----->H------+
      |                          |
      +--------------------------+

*/
TEST_F(LoopAnalyzerTest, TEST_6) {
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
    LinkBlocks(d, g);
    LinkBlocks(e, d);
    LinkBlocks(e, f);
    LinkBlocks(f, b);
    LinkBlocks(f, h);
    LinkBlocks(g, c);
    LinkBlocks(g, i);
    LinkBlocks(h, g);
    LinkBlocks(h, i);

    AnalyzeLoops();
    const auto &loops = analyzer_->GetLoops();

    ASSERT_GE(loops.size(), 1);

    const Loop *loopB = nullptr;
    const Loop *loopC = nullptr;

    for(const auto &loop: loops) {
        if(loop->GetHeader() == b) {
            loopB = loop.get();
        } else if(loop->GetHeader() == c) {
            loopC = loop.get();
        }
    }

    ASSERT_NE(loopB, nullptr);

    const auto &blocksB = loopB->GetBlocks();
    EXPECT_TRUE(blocksB.count(b));
    EXPECT_TRUE(blocksB.count(e));
    EXPECT_TRUE(blocksB.count(f));

    const auto &backEdgesB = loopB->GetBackEdges();
    ASSERT_GE(backEdgesB.size(), 1);
    EXPECT_EQ(backEdgesB[0], f);

    if(loopC != nullptr) {
        const auto &blocksC = loopC->GetBlocks();
        EXPECT_TRUE(blocksC.count(c));
        EXPECT_TRUE(blocksC.count(d));
        EXPECT_TRUE(blocksC.count(g));
    }
}