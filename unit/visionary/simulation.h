//
//
//
#ifndef __VISIONARY_SIMULATION_H
#define __VISIONARY_SIMULATION_H

#include <visionary/assertion.h>
#include <visionary/unit.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <array>
#include <unordered_map>

namespace visionary {


namespace detail {
class SimulationThread {
public:
  SimulationThread(SimulationThread& thr) = delete;
  SimulationThread& operator=(SimulationThread& thr) = delete;

  SimulationThread() { }
  template<typename Function, typename... Args>
  SimulationThread(Function&& funct, Args&&... args) : thr(::std::thread(std::forward<Function>(funct),
                                                                       std::forward<Args>(args)...)) { }

  SimulationThread(SimulationThread&& sim_thread) : thr(::std::move(sim_thread.thr)) { }

  SimulationThread& operator=(SimulationThread&& sim_thr) {
    thr = ::std::move(sim_thr.thr);
    return *this;
  }

  void Join() { thr.join(); }
  void Detach() { thr.detach(); }
  bool Joinable() { return thr.joinable(); }
private:
  ::std::thread thr;
};
} // detail

/**
  Simulation Enviroment is exactly as the name implies, It creates a simulated enviroment for testing 
  Objects and interacting via threads, to make a sort of network involving remote computers that are not
  near. We will soon expand the simulation to deal with processes, which would be much better to use than
  threads.
*/
template<uint32_t test_cases>
class SimulationEnviroment {
public:
  SimulationEnviroment() { }

  /**
    Constucts a Simulation Enviroment with the given test cases.
  */
  template<typename Function>
  SimulationEnviroment(std::array<Function, test_cases> tests) {
    AddSimulation(tests);
  }

  /**
    Waits for all threads to join when finished.
  */
  ~SimulationEnviroment() {
    std::for_each(simulations.begin(), simulations.end(), [] (detail::SimulationThread& thr) -> void {
      if (thr.Joinable()) {
        thr.Join();
      } else {
        thr.Detach();
      }
    });
  }

  /**
    A simulation must be in the form of an array of functions to be executed as if 
    there were multiple computers executing each function individually. 
  */
  template<typename Function>
  void AddSimulation(std::array<Function, test_cases> tests) {
    for (uint32_t t_i = 0; t_i < tests.size(); ++t_i) {
      simulations[t_i] = detail::SimulationThread(tests[t_i]);
    }
  }

  /**
    Don't need to call this yet.
  */
  void RunEnviroment() {
  }
  
private:
  uint32_t current_tab;
  std::array<detail::SimulationThread, test_cases> simulations;
};
} // visionary
#endif // __VISIONARY_SIMULATION_H