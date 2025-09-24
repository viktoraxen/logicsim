#include "gate.hpp"

#include "node.hpp"
#include "scheduler.hpp"

Gate::Gate(std::vector<Node*> inputs,
           std::vector<Node*> outputs,
           std::string name)
    : m_inputs(inputs)
    , m_outputs(outputs)
    , name(name)
{
    for (const auto& input : inputs)
        input->add_connection(this);
}

void Gate::initial_update()
{
    Scheduler scheduler;
    scheduler.schedule(this);
    scheduler.run();
}

void Gate::update(Scheduler& scheduler)
{
    bool new_value = compute();

    for (const auto& output : m_outputs)
        output->_set_value(new_value, scheduler);
}

bool AndGate::compute()
{
    return m_inputs[0]->read_value() and m_inputs[1]->read_value();
}

bool NorGate::compute()
{
    return not(m_inputs[0]->read_value() or m_inputs[1]->read_value());
}

bool NotGate::compute()
{
    return not m_inputs[0]->read_value();
}
