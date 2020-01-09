#include <iostream>
#include "ContainerTester/ContainerTester.h"
#include "Menu/Menu.h"
#include "Vector/Vector.h"
#include "HashedArrayTree/HashedArrayTree.h"
#include <vector>
#include <deque>

int main() {
	ContainerTester<std::deque<int>> stl_deque_tester;
	ContainerTester<std::vector<int>> stl_vector_tester;
	ContainerTester<Vector<int>> vector_tester;
	ContainerTester<HashedArrayTree<int>> arr_tree_tester;

	vector_tester.auto_test();
	arr_tree_tester.auto_test();
	stl_vector_tester.auto_test();
	stl_deque_tester.auto_test();
}