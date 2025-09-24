#include <iostream>

#include "gate.hpp"
#include "node.hpp"
#include "scheduler.hpp"

int main(int argc, char* argv[])
{
    Node S, R, Q, q;

    // Create the SR latch logic using cross-coupled NOR gates
    auto n1 = Gate::create<NorGate>(S, Q, q);
    auto n2 = Gate::create<NorGate>(R, q, Q);

    std::cout << "S " << S.read_value() << std::endl;
    std::cout << "R " << R.read_value() << std::endl;
    std::cout << "Q " << Q.read_value() << std::endl;
    std::cout << "q " << q.read_value() << std::endl;

    return 0;
}
