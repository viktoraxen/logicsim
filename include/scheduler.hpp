#pragma once

#include <queue>
#include <unordered_set>

class Gate;

class Scheduler
{
  public:
    Scheduler() {}
    ~Scheduler() = default;

    void run();
    void schedule(Gate* gate);
    void schedule(const std::vector<Gate*>& gate);

  private:
    std::queue<Gate*> m_queue;
    std::unordered_set<Gate*> m_set;
};
