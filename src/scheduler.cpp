#include "scheduler.hpp"

#include <cstring>

#include "gate.hpp"

void Scheduler::run()
{
    while (!m_queue.empty())
    {
        Gate* front = m_queue.front();
        front->update(*this);
        m_queue.pop();
        m_set.erase(front);
    }
}

void Scheduler::schedule(Gate* gate)
{
    if (m_set.count(gate) != 0)
        return;

    m_queue.push(gate);
    m_set.insert(gate);
}

void Scheduler::schedule(const std::vector<Gate*>& gates)
{
    for (const auto& gate : gates)
        schedule(gate);
}
