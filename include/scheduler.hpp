#pragma once

#include <queue>
#include <stdexcept>
#include <unordered_set>

class Gate;

#include <string>

class InvalidCircuitError : public std::runtime_error
{
  public:
    InvalidCircuitError(int num_updates)
        : std::runtime_error("Invalid circuit detected: " +
                             std::to_string(num_updates) + " updates")
    {}
};

class Scheduler
{
  public:
    /**
     * @brief Constructs a new Scheduler object.
     */
    Scheduler() {}
    ~Scheduler() = default;

    /**
     * @brief Runs the simulation.
     *
     * This method runs the simulation by updating all scheduled gates.
     * It continues to update gates until the queue is empty.
     */
    void run();

    /**
     * @brief Schedules a gate for update.
     *
     * @param gate The gate to schedule.
     */
    void schedule(Gate* gate);

    /**
     * @brief Schedules a vector of gates for update.
     *
     * @param gate The gates to schedule.
     */
    void schedule(const std::vector<Gate*>& gate);

    /**
     * @brief Clears the scheduler.
     *
     * This method clears the queue and the set of scheduled gates.
     */
    void clear();

  private:
    std::queue<Gate*> m_queue;
    std::unordered_set<Gate*> m_set;
};
