#include "test.h"
#include <visionary/simulation.h>
#include <iostream>
#include <chrono>


int cat() {
  
  return 0;
}


int main(int c, char* args[]) {
  

  std::array<std::function<void()>, 3> tests;
  tests[0] = [] () -> void {
    std::cout << "Hi, I'm a cat!" << std::endl; 
  };

  tests[1] = [] () -> void { 
    std::cout << "I am also a cat!" << std::endl;
  };

  tests[2] = [] () -> void {
    std::cout << "Guys, I'm a cat too!" << std::endl;
  };
  visionary::SimulationEnviroment<3> enviro(tests);

  std::cin.ignore();
  return 0;
}
