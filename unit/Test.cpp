#include <tools/Comparator.h>
#include <CommProto/Packets.h>
#include <tools/data_structures/SingleLinkedList.h>

#include <stdio.h>

using namespace Comnet;

namespace TestComparator {

class Test {
private:
  int32_t number;
  char letter;

  friend class TestCompare;
  
public:
  Test() : number(0), letter('a') { }
  Test(int32_t number) : number(number), letter('a') { }
  Test(int32_t number, char letter) : number(number), letter(letter) { }
};

class TestCompare : public Comparator<Test> {
  typedef const Test& const_reference;
public:

  TestCompare() { }
  ~TestCompare() { }
  // Compare object.
  int32_t compare(const_reference obj1, const_reference obj2) {
    if (obj1.number > obj2.number) {
      return 1;
    } else if (obj1.number < obj2.number) {
      return -1;
    } else {
      return 0;
    }
  }

  bool equal(const_reference obj1, const_reference obj2) {
    return (obj1.number == obj2.number);
  }

  //  int32_t operator()(const Test& obj1, const Test& obj2) {
  //  return compare(obj1, obj2);
  // }
};
} // TestComparator

class IntComparator : public Comparator<int> {
public:
  int32_t compare(const int& a, const int& b) {
    if (a > b) {
      return 1;
    } else if (a < b) {
      return -1;
    } else {
      return 0;
    }
  }

  bool equal(const int& a, const int& b) {
    return a == b;
  }
};

#include <iostream>
#include <string>

using namespace TestComparator;
using namespace Comnet;
using namespace std;

int main(int c, char** args) {
  Test t1(10);
  Test t2(20);

  CommsPacket packet(40, true);
  AbstractPacket& aPacket = packet;

  Comnet::DataStructure::SingleLinkedList<int, IntComparator> testList;
  cout << "List was created!!" << endl;
  testList.insert(1);
  testList.insert(4);
  testList.insert(19);
  cout << "List has added in values" << endl;
  cout << "Removing 1: " << testList.remove(1) << endl;
  cout << "Removing 10: " << testList.remove(10) << endl;
  cout << "Size of list: " << testList.getSize() << endl;


  CommsPacket cff = dynamic_cast<CommsPacket&>(aPacket);
  cout << cff.getId() << endl;
  //TestCompare tC;
  //Comparator<Test>& _cmp = tC;
  TestCompare tC;
  Comparator<Test>& _cmp = tC;
  int32_t result = _cmp(t1, t2);
  printf("%d\n", result);
  return 0;
}
