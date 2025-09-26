#include <gtest/gtest.h>

#include "gate.hpp"
#include "node.hpp"
#include "scheduler.hpp"

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

TEST(Latches, SR_InvalidState)
{
    Node S, R, Q, q;

    auto n1 = Gate::create<NorGate>(R, q, Q); // Q output
    auto n2 = Gate::create<NorGate>(S, Q, q); // q output

    S.set_value(false);
    R.set_value(true);
    R.set_value(false);

    ASSERT_EQ(Q.read_value(), false);
    ASSERT_EQ(q.read_value(), true);

    S.set_value(true);
    R.set_value(true);

    // For a NOR latch, the expected (though invalid) output is Q=0 and q=0.
    EXPECT_EQ(Q.read_value(), false);
    EXPECT_EQ(q.read_value(), false);

    // Releasing R first should result in a SET state (Q=1, q=0)
    R.set_value(false);
    EXPECT_EQ(Q.read_value(), true);
    EXPECT_EQ(q.read_value(), false);

    // Reset to invalid state
    R.set_value(true);
    EXPECT_EQ(Q.read_value(), false);
    EXPECT_EQ(q.read_value(), false);

    // Releasing S first should result in a RESET state (Q=0, q=1)
    S.set_value(false);
    EXPECT_EQ(Q.read_value(), false);
    EXPECT_EQ(q.read_value(), true);
}

/*
 * Verifies that the JK latch correctly throws an `InvalidCircuitError`
 * when both J and K inputs are high, which is an invalid state.
 */
TEST(Latches, JK_invalid)
{
    Node J, K, Q, q, C;
    Node int1, int2;
    Node ext1, ext2;

    auto a2 = Gate::create<AndGate>(K, Q, int1);
    auto a1 = Gate::create<AndGate>(J, q, int2);

    auto a3 = Gate::create<AndGate>(int1, C, ext1);
    auto a4 = Gate::create<AndGate>(int2, C, ext2);

    auto n1 = Gate::create<NorGate>(ext1, q, Q);
    auto n2 = Gate::create<NorGate>(ext2, Q, q);

    K.set_value(true);
    J.set_value(true);

    EXPECT_THROW(C.set_value(true), InvalidCircuitError);
}

TEST(Latches, GatedSR)
{
    Node S, R, E, Q, q;
    Node S_gated, R_gated;

    // Gating logic
    auto a1 = Gate::create<AndGate>(S, E, S_gated);
    auto a2 = Gate::create<AndGate>(R, E, R_gated);

    // Core SR Latch
    auto n1 = Gate::create<NorGate>(R_gated, q, Q);
    auto n2 = Gate::create<NorGate>(S_gated, Q, q);

    bool initial_Q = Q.read_value();
    bool initial_q = q.read_value();

    // Case 1: Enable is low. S and R changes should be ignored.
    E.set_value(false);
    S.set_value(true);

    EXPECT_EQ(Q.read_value(), initial_Q);
    EXPECT_EQ(q.read_value(), initial_q);

    S.set_value(false);
    R.set_value(true);

    EXPECT_EQ(Q.read_value(), initial_Q); // Still no change
    EXPECT_EQ(q.read_value(), initial_q);

    // Case 2: Enable is high. Latch should be transparent.
    E.set_value(true);
    R.set_value(false);
    S.set_value(true);

    EXPECT_EQ(Q.read_value(), true);
    EXPECT_EQ(q.read_value(), false);

    S.set_value(false); // Hold
    EXPECT_EQ(Q.read_value(), true);
    EXPECT_EQ(q.read_value(), false);

    R.set_value(true);
    EXPECT_EQ(Q.read_value(), false);
    EXPECT_EQ(q.read_value(), true);

    // Case 3: Enable goes low while in a set state. It should hold this state.
    R.set_value(false);
    S.set_value(true);
    EXPECT_EQ(Q.read_value(), true); // Verify it's set
    EXPECT_EQ(q.read_value(), false);

    E.set_value(false); // Latch the state

    S.set_value(false); // Attempt to change inputs
    R.set_value(true);

    EXPECT_EQ(Q.read_value(), true); // Should hold the latched 'set' state
    EXPECT_EQ(q.read_value(), false);
}

TEST(FlipFlops, MasterSlaveJK)
{
    // --- Inputs ---
    Node J("J"), K("K"), C("C");

    // --- Outputs ---
    Node Q("Q"), q(true, "q"); // Start with q=1, Q=0

    // --- Internal nodes ---
    Node Cn("Cn");                 // Inverted Clock
    Node Qm("Qm"), qm(true, "qm"); // Master latch outputs

    // 1. Invert the clock for the slave latch
    auto clk_inverter = Gate::create<NotGate>(C, Cn, "clk_inverter");

    // 2. Master Latch (responds when C is HIGH)
    Node master_S_in("master_S_in"), master_R_in("master_R_in");
    auto m_s_in_gate = Gate::create<AndGate>(J, q, master_S_in);
    auto m_r_in_gate = Gate::create<AndGate>(K, Q, master_R_in);
    Node master_S("master_S"), master_R("master_R");
    auto m_s_gate = Gate::create<AndGate>(master_S_in, C, master_S);
    auto m_r_gate = Gate::create<AndGate>(master_R_in, C, master_R);
    auto m_nor1 = Gate::create<NorGate>(master_R, qm, Qm);
    auto m_nor2 = Gate::create<NorGate>(master_S, Qm, qm);

    // 3. Slave Latch (responds when C is LOW, i.e., Cn is HIGH)
    Node slave_S("slave_S"), slave_R("slave_R");
    auto s_s_gate = Gate::create<AndGate>(Qm, Cn, slave_S);
    auto s_r_gate = Gate::create<AndGate>(qm, Cn, slave_R);
    auto s_nor1 = Gate::create<NorGate>(slave_R, q, Q);
    auto s_nor2 = Gate::create<NorGate>(slave_S, Q, q);

    // --- Simulation & Assertions ---

    // Initial State: Q=0, q=1
    ASSERT_EQ(Q.read_value(), false);
    ASSERT_EQ(q.read_value(), true);

    // 1. Test Set (J=1, K=0)
    J.set_value(true);
    K.set_value(false);

    // Pulse clock: High then Low
    C.set_value(true);  // Master reads J=1, K=0 -> Qm=1, qm=0
    C.set_value(false); // Slave copies master -> Q=1, q=0
    EXPECT_EQ(Q.read_value(), true);
    EXPECT_EQ(q.read_value(), false);

    // 2. Test Reset (J=0, K=1)
    J.set_value(false);
    K.set_value(true);

    // Pulse clock
    C.set_value(true);  // Master reads J=0, K=1 -> Qm=0, qm=1
    C.set_value(false); // Slave copies master -> Q=0, q=1
    EXPECT_EQ(Q.read_value(), false);
    EXPECT_EQ(q.read_value(), true);

    // 3. Test Toggle (J=1, K=1) - First toggle
    J.set_value(true);
    K.set_value(true);

    // Pulse clock
    C.set_value(true);  // Master sees Q=0, q=1. Toggles -> Qm=1, qm=0
    C.set_value(false); // Slave copies master -> Q=1, q=0
    EXPECT_EQ(Q.read_value(), true);
    EXPECT_EQ(q.read_value(), false);

    // 4. Test Toggle (J=1, K=1) - Second toggle
    // Pulse clock again
    C.set_value(true);  // Master sees Q=1, q=0. Toggles -> Qm=0, qm=1
    C.set_value(false); // Slave copies master -> Q=0, q=1
    EXPECT_EQ(Q.read_value(), false);
    EXPECT_EQ(q.read_value(), true);
}
