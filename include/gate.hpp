#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

class Node;
class Scheduler;

class Gate
{
  public:
    /**
     * @brief Creates a new gate of a specified type.
     *
     * This template function creates a new gate of a specified type and
     * performs an initial update.
     *
     * @tparam T The type of gate to create.
     * @param args The arguments to the gate's constructor.
     * @return A unique pointer to the newly created gate.
     */
    template <typename T, typename... Args>
    static std::unique_ptr<T> create(Args&&... args)
    {
        auto gate = std::make_unique<T>(std::forward<Args>(args)...);
        gate->initial_update();
        return gate;
    }

    /**
     * @brief Constructs a new Gate object.
     *
     * @param inputs A vector of pointers to the input nodes.
     * @param outputs A vector of pointers to the output nodes.
     * @param name The name of the gate.
     */
    Gate(std::vector<Node*> inputs,
         std::vector<Node*> outputs,
         std::string name);
    ~Gate() = default;

    /**
     * @brief Updates the gate's output based on its inputs.
     *
     * This method is called by the scheduler to update the gate's output.
     * It calls the compute method to determine the new output value and
     * then sets the output nodes to the new value.
     *
     * @param scheduler The scheduler to use for scheduling updates.
     */
    void update(Scheduler& scheduler);

  protected:
    /**
     * @brief Computes the gate's output value.
     *
     * This pure virtual method is implemented by each gate type to compute
     * its output value based on its input values.
     *
     * @return The computed output value.
     */
    virtual bool compute() = 0;

    /**
     * @brief Performs the initial update of the gate.
     *
     * This method is called when the gate is created to perform the initial
     * update of the gate's output.
     */
    void initial_update();

    std::vector<Node*> m_inputs;
    std::vector<Node*> m_outputs;

  public:
    std::string m_name;
};

class AndGate : public Gate
{
  public:
    AndGate(Node& i1, Node& i2, Node& o, std::string name = "")
        : Gate({&i1, &i2}, {&o}, name)
    {}

  private:
    bool compute() override;
};

class NorGate : public Gate
{
  public:
    NorGate(Node& i1, Node& i2, Node& o, std::string name = "")
        : Gate({&i1, &i2}, {&o}, name)
    {}

  private:
    bool compute() override;
};

class NotGate : public Gate
{
  public:
    NotGate(Node& i1, Node& o, std::string name = "")
        : Gate({&i1}, {&o}, name)
    {}

  private:
    bool compute() override;
};
