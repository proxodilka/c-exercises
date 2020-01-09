#pragma once
#include "../exceptions.h"

//TODO:
//	1. fix:
//		a) resize doesn't work properly
//	2. implement:
//		a) pop_back
//		b) removeAt
//		c) reserve
//	3. add:
//		a) ability to work with custom allocator
//		b) move semantic support


template<typename T>
class HashedArrayTree {
	T** roots;
	size_t* arr_sizes;
	size_t _size, arr_capacity_base, roots_size, arr_capacity;

	void inc_capacity() {
		arr_capacity_base++;
		arr_capacity = (1 << arr_capacity_base);
	}

	void set_capacity_base(size_t value) {
		arr_capacity_base = value;
		arr_capacity = (1 << arr_capacity_base);
	}

	void destructContainedObjects() {
		for (size_t i = 0; i < this->size(); i++) {
			(*this)[i].~T();
		}
	}

	T* getPointerByIndex(size_t index) {
		size_t root_n = index / (1 << arr_capacity_base);
		size_t pos_n = index % (1 << arr_capacity_base);
		return (roots[root_n] + pos_n);
	}

	void force_dispose() {
		destructContainedObjects();
		for (size_t i = 0; i < roots_size; i++) {
			operator delete[](roots[i]);
		}
		delete[] roots;
		roots = nullptr;

		set_capacity_base(1);
		_size = 0;
		roots_size = 0;
	}

	void init_roots() {
		roots = new T * [arr_capacity];
	}

	void resize(size_t capacity_base) {
		size_t new_arr_capacity = 1 << capacity_base;
		T** new_roots = new T * [new_arr_capacity];
		size_t new_root_ind = 0, new_pos_ind = new_arr_capacity;

		for (size_t i = 0; i < this->size(); i++, new_pos_ind++) {
			if (new_pos_ind == new_arr_capacity) {
				new_roots[new_root_ind++] = static_cast<T*>(operator new[](sizeof(T)* new_arr_capacity));
				new_pos_ind = 0;
			}
			new (new_roots[new_root_ind - 1] + new_pos_ind) T(std::move((*this)[i]));
		}

		size_t old_capacity_base = arr_capacity_base;
		size_t old_size = _size;
		force_dispose();

		roots = new_roots;
		set_capacity_base(old_capacity_base + 1);
		_size = old_size;

		roots_size = arr_capacity / 4;

		//TODO: new capacity base may be less than current, in that case method wont work properly, fix this issue
	}

	void push_new_leaf() {
		roots[roots_size] = static_cast<T*>(operator new(sizeof(T) * arr_capacity));
		roots_size++;
	}

public:

	class iterator;
	class const_iterator;

	friend iterator;
	friend const_iterator;

	HashedArrayTree() {
		_size = 0;
		set_capacity_base(1);
		roots_size = 0;
		init_roots();
	}

	~HashedArrayTree() {
		force_dispose();
	}

	void dispose() {
		force_dispose();
		init_roots();
	}

	void expand() {
		resize(arr_capacity_base + 1);
	}

	void reserve(size_t amount) {
		throw not_implemented_exception();
	}

	void pop_back() {
		throw not_implemented_exception();
	}

	void removeAt(size_t index) {
		throw not_implemented_exception();
	}

	void push_back(const T& value) {
		size_t root_n = _size / arr_capacity;
		size_t pos_n = _size % arr_capacity;

		if (root_n == roots_size) {
			if (roots_size == arr_capacity) {
				expand();
				push_back(value);
				return;
			}
			push_new_leaf();
		}

		new (roots[root_n] + pos_n) T(value);
		_size++;
	}

	T& operator[](size_t index) {
		if (index >= _size) {
			throw std::invalid_argument("Index does not exist");
		}
		size_t root_n = index / (1 << arr_capacity_base);
		size_t pos_n = index % (1 << arr_capacity_base);
		return *(roots[root_n] + pos_n);
	}

	size_t size() {
		return _size;
	}

	iterator begin() {
		return iterator(0, this);
	}

	iterator end() {
		return iterator(this->size(), this);
	}

	const_iterator cbegin() {
		return const_iterator(0, this);
	}

	const_iterator cend() {
		return const_iterator(this->size(), this);
	}

};

template<typename T>
class HashedArrayTree<T>::iterator : public std::iterator<std::random_access_iterator_tag, T> {
protected:
	friend HashedArrayTree;
	T* pointer;
	HashedArrayTree* sourceHAT;
	size_t index;

	iterator(size_t index, HashedArrayTree* sourceHAT): pointer(sourceHAT->getPointerByIndex(index)), index(index), sourceHAT(sourceHAT) {}
	
public:

	T& operator*() { return *(this->pointer); }
	T& operator[](int offset) { return *(sourceHAT->getPointerByIndex(index + offset)); }

	iterator operator+(int value) { return iterator(index + value, sourceHAT); }
	iterator operator-(int value) { return iterator(index - value, sourceHAT); }

	bool operator==(const iterator& it) { return it.pointer == pointer; }
	bool operator!=(const iterator& it) { return it.pointer != pointer; }
	bool operator<(const iterator& it) { return pointer < it.pointer; }
	bool operator>(const iterator& it) { return pointer > it.pointer; }
	bool operator<=(const iterator& it) { return pointer <= it.pointer; }
	bool operator>=(const iterator& it) { return pointer >= it.pointer; }

	iterator& operator++(int) { pointer = sourceHAT->getPointerByIndex(++index); return *this; }
	iterator& operator--(int) { pointer = sourceHAT->getPointerByIndex(--index); return *this; }

	iterator operator++() {
		iterator retValue(*this);
		pointer = sourceHAT->getPointerByIndex(++index);
		return retValue;
	}

	iterator operator--() {
		iterator retValue(*this);
		pointer = sourceHAT->getPointerByIndex(--index);
		return retValue;
	}

	iterator& operator+=(int value) { pointer = sourceHAT->getPointerByIndex(index += value); return *this; }
	iterator& operator-=(int value) { pointer = sourceHAT->getPointerByIndex(index -= value); return *this; }
};

template<typename T>
class HashedArrayTree<T>::const_iterator : public iterator {
	friend HashedArrayTree;
	const_iterator(size_t index, HashedArrayTree *sourceHAT): iterator(index, sourceHAT) {}

public:
	const_iterator(const iterator& other): iterator(other.index, other.sourceHAT) {}

	const T& operator*() { return *(this->pointer); }
	const T& operator[](int offset) { return *(this->sourceHAT->getPointerByIndex(this->index + offset)); }
};

