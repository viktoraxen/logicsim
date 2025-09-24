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
    template <typename T, typename... Args>
    static std::unique_ptr<T> create(Args&&... args)
    {
        auto gate = std::make_unique<T>(std::forward<Args>(args)...);
        gate->initial_update();
        return gate;
    }

    Gate(std::vector<Node*> inputs,
         std::vector<Node*> outputs,
         std::string name);
    ~Gate() = default;

    void update(Scheduler& scheduler);

  protected:
    virtual bool compute() = 0;
    void initial_update();

    std::vector<Node*> m_inputs;
    std::vector<Node*> m_outputs;

  public:
    std::string name;
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
