#include "node.hpp"

#include <iostream>

#include "gate.hpp"
#include "scheduler.hpp"

Node::Node(bool value, std::string name)
    : m_value(value)
    , m_name(name)
{}

void Node::add_connection(Gate* gate)
{
    m_connections.push_back(gate);
}

void Node::set_value(bool v)
{
    if (v == m_value)
        return;

    Scheduler scheduler;

    _set_value(v, scheduler);

    scheduler.run();
}

void Node::_set_value(bool v, Scheduler& scheduler)
{
    if (v == m_value)
        return;

    // std::cout << m_name << ": set to " << v << std::endl;

    m_value = v;
    scheduler.schedule(m_connections);
}
