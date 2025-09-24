#include <gtest/gtest.h>

#include "gate.hpp"
#include "node.hpp"

TEST(Latches, D)
{
    Node D, E, Q, q, A, B, C;

    auto n = Gate::create<NotGate>(D, A);
    auto a1 = Gate::create<AndGate>(A, E, B),
         a2 = Gate::create<AndGate>(D, E, C);
    auto n1 = Gate::create<NorGate>(B, q, Q),
         n2 = Gate::create<NorGate>(C, Q, q);

    // E high->Q = D, q = !D
    E.set_value(true);
    D.set_value(true);

    EXPECT_EQ(Q.read_value(), true);
    EXPECT_EQ(q.read_value(), false);

    D.set_value(false);

    EXPECT_EQ(Q.read_value(), false);
    EXPECT_EQ(q.read_value(), true);

    // E low->Q, q does not change value
    E.set_value(false);

    EXPECT_EQ(Q.read_value(), false);
    EXPECT_EQ(q.read_value(), true);

    D.set_value(true);

    EXPECT_EQ(Q.read_value(), false);
    EXPECT_EQ(q.read_value(), true);
}

TEST(Latches, SR)
{
    Node S, R, Q, q;

    auto n1 = Gate::create<NorGate>(S, Q, q);
    auto n2 = Gate::create<NorGate>(R, q, Q);

    // S high -> Set
    S.set_value(true);

    EXPECT_EQ(Q.read_value(), true);
    EXPECT_EQ(q.read_value(), false);

    // S, R low -> Hold previous
    S.set_value(false);

    EXPECT_EQ(Q.read_value(), true);
    EXPECT_EQ(q.read_value(), false);

    // R high -> Reset
    S.set_value(false);
    R.set_value(true);

    EXPECT_EQ(Q.read_value(), false);
    EXPECT_EQ(q.read_value(), true);

    // S, R low -> Hold previous
    R.set_value(false);

    EXPECT_EQ(Q.read_value(), false);
    EXPECT_EQ(q.read_value(), true);
}
