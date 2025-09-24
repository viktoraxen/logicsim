#include <iostream>

#include "gate.hpp"
#include "node.hpp"
#include "scheduler.hpp"

int main(int argc, char* argv[])
{
    Node D, E, Q, q, A, B, C;

    auto n = Gate::create<NotGate>(D, A);
    auto a1 = Gate::create<AndGate>(A, E, B),
         a2 = Gate::create<AndGate>(D, E, C);
    auto n1 = Gate::create<NorGate>(B, q, Q),
         n2 = Gate::create<NorGate>(C, Q, q);

    E.set_value(true);
    D.set_value(true);

    std::cout << "Q  " << Q.read_value() << std::endl;
    std::cout << "q  " << q.read_value() << std::endl;

    D.set_value(false);

    std::cout << "Q  " << Q.read_value() << std::endl;
    std::cout << "q  " << q.read_value() << std::endl;

    return 0;
}
