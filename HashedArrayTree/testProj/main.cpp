#include <iostream>
#include <utility>
#include "C:\Users\rp-re\OneDrive\Desktop\labs\labs-c-plus-plus\lab3_05_dchigarev\vectorLaba\Vector\Vector.h"
//#include "C:\Users\rp-re\OneDrive\Desktop\labs\labs-c-plus-plus\lab3_05_dchigarev\vectorLaba\DummyAllocator\DummyAllocator.h"
#include "C:\Users\rp-re\OneDrive\Desktop\labs\labs-c-plus-plus\lab3_05_dchigarev\vectorLaba\Complex\Complex.h"

using namespace std;

template<typename T>
class List {
private:
	struct Node {
		Node(T value) {
			this->value = value;
			this->next = nullptr;
		}

		T value;
		Node* next;
	};
	
	Node* begin;
	Node* end;
	size_t size;

public:
	List():begin(nullptr), end(nullptr) {}

	void push_back(const T& value) {
		Node* new_node = new Node(value);
		if (end == nullptr) {
			begin = end = new_node;
		}
		else {
			end->next = new_node;
			end = new_node;
		}
		size++;
	}

	void reverse() {
		Node* prev = nullptr, *cur = begin;
		while (cur) {
			Node* buf = cur->next;
			cur->next = prev;
			prev = cur;
			cur = buf;
		}
		swap(begin, end);
	}

	friend ostream& operator<<(ostream& out, const List& obj) {
		Node* cur = obj.begin;
		while (cur) {
			out << cur->value << " ";
			cur = cur->next;
		}
		return out;
	}

};

#include <typeinfo>

class Base {
	virtual void f() {};
};

class Derived :public Base {
	void f() {};
};

int& f(int x) {
	int y = x + 1;
	return y;
}

template<typename T, typename U>
struct Is_same { const static bool value = false; };

template<typename T>
struct Is_same<T,T> { const static bool value = true; };

template<typename T>
struct Remove_reference { typedef T value; };

template<typename T>
struct Remove_reference<T*> { typedef T value; };

class Test {
public:
	Test() { cout << "default const\n"; };
	Test(const Test& other) { cout << "copy\n"; }
	//Test(Test&& other) { cout << "rvalue copy\n"; }
};

template<typename T, class Alloc = std::allocator<T>>
class Shared {
	struct shared_struct {
		T* ptr;
		size_t count;
	};
	shared_struct* h;
	Alloc allocator;
public:
	Shared(T* ptr, Alloc allocator = Alloc()) {
		
		h = new shared_struct;
		h->ptr = ptr;
		h->count = 1;
		
	}

	Shared(const Shared& other) {
		h = other.h;
		h->count++;
	}

	Shared& operator=(const Shared& other) = delete;

	~Shared() {
		h->count--;
		if (h->count == 0) {
			delete h->ptr;
		}
	}
};

int rev(int a, int b=0)
{
	if (a < 10) return b * 10 + a;
	return rev(a / 10, b * 10 + a % 10);
}

size_t reverse_int_rec(size_t number) {
	if (number<10) return number;
	size_t ans = 0;
	ans += number%10+reverse_int_rec(number / 10);
	ans *= 10;
	return ans;
}

size_t reverse_int(size_t number) {
	size_t ans = 0;
	while (number) {
		ans += number % 10;
		number /= 10;
		ans *= number?10:1;
	}
	return ans;
}

template<size_t SIZE, typename KeyType, typename ValueType, class Hash, class Equal = std::equal_to<KeyType>>
class HashMap {
	ValueType* data;
};

template<typename T>
class HashedArrayTreeLeg {
	//typedef std::allocator_traits<allocator_type>::rebind_alloc<Vector<T, allocator_type>> root_allocator;
	//root_allocator allocator;
	Vector < Vector<T> > roots;
	size_t _size, coef, calcedPow, cur_ptr;
public:

	HashedArrayTreeLeg(){
		_size = 0;
		coef = 2;
		cur_ptr = 0;
		roots.restrict_memory_releasing = true;
		roots.reserve(1 << (coef-1));
	}

	void expand() {
		coef += 1;
		roots.reserve(1 << coef);
		for (Vector<T>& arr : roots) {
			arr.reserve(1 << coef);
		}
		size_t ind_to_copy = 0;
		for (int32_t i = 0; i < (int32_t)roots.size() - 1; i++) {
			for (T& value : roots[i+1]) {
				roots[ind_to_copy].push_back(std::move(value));
			}
			roots[i + 1].clear();
			if (roots[ind_to_copy].size() == (1 << coef)) {
				ind_to_copy++;
			}
		}
		roots.removeAll([](const Vector<T>& v) {return v.size() == 0; });
		cur_ptr = roots.size();
	}

	void push_back(const T& value) {
		size_t root_n = _size / (1 << (coef));
		size_t pos_n = _size % (1 << (coef));

		if (root_n == cur_ptr) {
			if (cur_ptr == (1<<coef) ) {
				expand();
				push_back(value);
				return;
			}
			roots.push_back(Vector<T>(1<<coef, true));
			cur_ptr++;
		}

		roots[root_n].push_back(value);
		_size++;
	}

	T& operator[](size_t index) {
		if (index >= _size) {
			throw std::invalid_argument("Index does not exist");
		}
		size_t root_n = index / (1 << coef);
		size_t pos_n = index % (1 << coef);
		return roots[root_n][pos_n];
	}

	size_t size() {
		return _size;
	}

};



#include <chrono>

std::chrono::milliseconds get_time() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

#include <vector>
#include "HashedArrayTree.h"
//void t1() {
//	HashedArrayTree<int> v;
//	auto v_s = get_time();
//	for (int i = 0; i < 6e6; i++) {
//		v.push_back(i);
//	}
//	auto v_e = get_time();
//	cout << "HashedArrayTree based on Vector: " << std::chrono::duration<double>(v_e - v_s).count() << "s.\n";
//	//system("pause");
//}
//
//void t2() {
//	Vector<int> v1;
//	auto v1_s = get_time();
//	for (int i = 0; i < 6e6; i++) {
//		v1.push_back(i);
//	}
//	auto v1_e = get_time();
//	cout << "Vector: " << std::chrono::duration<double>(v1_e - v1_s).count() << "s.\n";
//	//system("pause");
//}
//
//void t3() {
//	vector<int> v2;
//	auto v2_s = get_time();
//	for (int i = 0; i < 6e6; i++) {
//		v2.push_back(i);
//	}
//	auto v2_e = get_time();
//	cout << "std::vector: " << std::chrono::duration<double>(v2_e - v2_s).count() << "s.\n";
//	//system("pause");
//}
//
////void t4() {
////	HashedArrayTreePure<int> v;
////	auto v2_s = get_time();
////	for (int i = 0; i < 6e6; i++) {
////		v.push_back(i);
////	}
////	auto v2_e = get_time();
////	cout << "HashedArrayTree: " << std::chrono::duration<double>(v2_e - v2_s).count() << "s.\n";
////	//system("pause");
////}
////
////void t5(int n) {
////	HashedArrayTreePure<int> v;
////	for (int i = 0; i < n; i++) {
////		v.push_back(i);
////	}
////	for (int i = 0; i < v.size(); i++) {
////		cout << v[i] << " ";
////	}
////}
//
//void t6() {
//	HashedArrayTree<int> v;
//	for (int i = 0; i < 513; i++) {
//		v.push_back(i);
//	}
//	for (int i = 0; i < v.size(); i++) {
//		cout << v[i] << " ";
//	}
//}
//
//void t7() {
//	auto v2_s = get_time();
//	int* arr = new int[6e6];
//	for (int i = 0; i < 6e6; i++) {
//		arr[i] = i;
//	}
//	auto v2_e = get_time();
//	cout << "Pure array: " << std::chrono::duration<double>(v2_e - v2_s).count() << "s.\n";
//	//system("pause");
//}

void stress() {
	HashedArrayTree<int> v;
	for (int i = 0; i < 12e6; i++) {
		v.push_back(i);
	}
	system("pause");
}

int main() {
	system("pause");
	stress();
	system("pause");
	/*HashedArrayTree<int> v;
	for (int i = 0; i < 3e6; i++) {
		v.push_back(i);
	}
	for (int i = 0; i < v.size(); i++) {
		cout << v[i] << " ";
	}*/
	//t6();
	//t5();
	/*t4();
	t7();
	t1();
	t2();
	t3();*/
}