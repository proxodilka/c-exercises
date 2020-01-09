**This class provides you an ability to test your container.
It obtains a container type as a template argument and rebinds contained type to class `Complex`.**

Class provides functional to test some main methods of sequence containers, note that all container
methods will be invoked through `ContainerTraits`, see `ContainerTraits.h` for details.

Usage examplex:
	
```
#include <iostream>
#include <vector>
#include "ContainerTester.h"

int main() {
  ContainerTester< std::vector<int> > tester;

  tester.fill_test();
  tester.pop_back_test();
  tester.pop_front_test();

  tester.auto_test();
  return 0;
}
```
