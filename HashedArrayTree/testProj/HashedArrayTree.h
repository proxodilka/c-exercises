#pragma once

template<typename T>
class HashedArrayTree {
	T** roots;
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

public:

	HashedArrayTree() {
		_size = 0;
		set_capacity_base(1);
		roots_size = 0;
		roots = new T * [arr_capacity];
	}

	~HashedArrayTree() {
		dispose();
	}

	void dispose() {
		destructContainedObjects();
		for (size_t i = 0; i < roots_size; i++) {
			operator delete[](roots[i]);
		}
		delete[] roots;
		roots = nullptr;
	}

	void expand() {
		size_t new_arr_capacity = 1 << (arr_capacity_base + 1);
		T** new_roots = new T * [new_arr_capacity];
		size_t new_root_ind = 0, new_pos_ind = new_arr_capacity;

		for (size_t i = 0; i < this->size(); i++, new_pos_ind++) {
			if (new_pos_ind == new_arr_capacity) {
				new_roots[new_root_ind++] = static_cast<T*>(operator new[](sizeof(T) * new_arr_capacity));
				new_pos_ind = 0;
			}
			new (new_roots[new_root_ind - 1] + new_pos_ind) T(std::move((*this)[i]));
		}

		dispose();
		roots = new_roots;
		inc_capacity();

		roots_size = arr_capacity / 4;
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
			roots[roots_size] = static_cast<T*>(operator new(sizeof(T) * arr_capacity));
			roots_size++;
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

};
