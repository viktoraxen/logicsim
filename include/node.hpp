#pragma once

#include <string>
#include <vector>

class Scheduler;
class Gate;

class Node
{
  public:
    /**
     * @brief Constructs a new Node object.
     *
     * @param value The initial value of the node.
     * @param name The name of the node.
     */
    Node(bool value = false, std::string name = "");
    ~Node() = default;

    /**
     * @brief Adds a connection to a gate.
     *
     * This method is called by the Gate constructor to add a connection
     * from this node to the gate.
     *
     * @param gate The gate to connect to.
     */
    void add_connection(Gate* gate);

    /**
     * @brief Sets the value of the node.
     *
     * This method sets the value of the node and schedules an update of
     * all connected gates.
     *
     * @param v The new value of the node.
     */
    void set_value(bool v);

    /**
     * @brief Reads the value of the node.
     *
     * @return The value of the node.
     */
    bool read_value() { return m_value; }

  private:
    std::vector<Gate*> m_connections;
    bool m_value;

    /**
     * @brief Sets the value of the node without scheduling an update.
     *
     * This method is called by the Gate class to set the value of the node
     * without scheduling an update. This is used to prevent infinite
     * recursion when a gate updates its output nodes.
     *
     * @param v The new value of the node.
     * @param scheduler The scheduler to use for scheduling updates.
     */
    void _set_value(bool v, Scheduler& scheduler);

    friend class Gate;
    friend class Scheduler;

  public:
    std::string m_name;
};
