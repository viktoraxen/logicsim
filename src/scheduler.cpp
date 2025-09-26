#include "scheduler.hpp"

#include <cmath>
#include <cstring>
#include <math.h>
#include <unordered_set>

#include "gate.hpp"

void Scheduler::run()
{
    std::unordered_set<Gate*> unique_gates;
    int num_updates = 0;
    int max_updates = 10;

    while (!m_queue.empty())
    {
        Gate* front = m_queue.front();

        if (unique_gates.count(front) == 0)
        {
            unique_gates.insert(front);
            max_updates = 50 + 5 * pow(unique_gates.size(), 2);
        }

        if (num_updates >= max_updates)
        {
            clear();
            throw InvalidCircuitError(num_updates);
        }

        front->update(*this);
        m_queue.pop();
        m_set.erase(front);

        num_updates++;
    }
}

void Scheduler::schedule(Gate* gate)
{
    if (m_set.count(gate) != 0)
        return;

    // std::cout << "Scheduling gate " << gate->m_name << std::endl;
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // char temp;
    // std::cin.get(temp);

    m_queue.push(gate);
    m_set.insert(gate);
}

void Scheduler::schedule(const std::vector<Gate*>& gates)
{
    for (const auto& gate : gates)
        schedule(gate);
}

void Scheduler::clear()
{
    m_queue = {};
    m_set = {};
}
