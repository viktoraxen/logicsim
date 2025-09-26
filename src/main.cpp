#include <iostream>

#include "gate.hpp"
#include "node.hpp"

int main(int argc, char* argv[])
{
    Node J(false, "J"), K(false, "K"), Q(false, "Q"), q(false, "q"),
        C(false, "C");
    Node int1(false, "int1"), int2(false, "int2");
    Node ext1(false, "ext1"), ext2(false, "ext2");

    auto a2 = Gate::create<AndGate>(K, Q, int1, "K and Q");
    auto a1 = Gate::create<AndGate>(J, q, int2, "J and q");

    auto a3 = Gate::create<AndGate>(int1, C, ext1, "C and int1");
    auto a4 = Gate::create<AndGate>(int2, C, ext2, "C and int2");

    auto n1 = Gate::create<NorGate>(ext1, q, Q, "q and ext1");
    auto n2 = Gate::create<NorGate>(ext2, Q, q, "Q and ext2");

    K.set_value(true);
    J.set_value(true);
    C.set_value(true);

    std::cout << "J  " << J.read_value() << std::endl;
    std::cout << "K  " << K.read_value() << std::endl;
    std::cout << "C  " << C.read_value() << std::endl;
    std::cout << "i1 " << int1.read_value() << std::endl;
    std::cout << "i2 " << int2.read_value() << std::endl;
    std::cout << "e1 " << ext1.read_value() << std::endl;
    std::cout << "e2 " << ext2.read_value() << std::endl;
    std::cout << "Q  " << Q.read_value() << std::endl;
    std::cout << "q  " << q.read_value() << std::endl;

    return 0;
}

