#include <tools/Comparator.hpp>

#include <stdio.h>

using namespace Comnet;

namespace TestComparator {

class Test : public Comparator<Test> {
public:
  Test() : number(0), letter('a') { }
  Test(int32_t number) : number(number), letter('a') { }
  Test(int32_t number, char letter) : number(number), letter(letter) { }

  int32_t compare(const Test& t1, const Test& t2) { 
    if (t1.number > t2.number) {
      return 1;
    } else if (t1.number == t2.number) {
      return 0;
    } else {
      return -1;
    }
  }
private:

  int32_t number;
  char letter;
};
} // TestComparator

using namespace TestComparator;

int main(int c, char** args) {
  Test t1(10);
  Test t2(20);
  Comparator<Test> _cmp;
  int32_t result = _cmp(t1, t2);
  printf("%d\n", result);
  return 0;
}
