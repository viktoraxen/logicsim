#pragma once

#include <string>
#include <vector>

class Scheduler;
class Gate;

class Node
{
  public:
    Node(bool value = false, std::string name = "");
    ~Node() = default;

    void add_connection(Gate* gate);

    void set_value(bool v);
    bool read_value() { return m_value; }

  private:
    std::vector<Gate*> m_connections;
    bool m_value;

    void _set_value(bool v, Scheduler& scheduler);

    friend class Gate;
    friend class Scheduler;

  public:
    std::string name;
};
