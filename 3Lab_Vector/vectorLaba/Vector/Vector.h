#pragma once
#include <functional>

#define min(a,b) (((a)<(b))?(a):(b))


template<typename T>
class Vector {
private:
	const static size_t max_size = INT_MAX;
	size_t _size, capacity;
	T* data;

	void resize(size_t new_size) { 

		T* new_data = static_cast<T*>(operator new[](sizeof(T)*new_size)); //allocate memory without constructing objects
		for (size_t i = 0; i < min(_size, new_size); i++) {
			new (new_data + i) T(data[i]); //construct new object at (new_data+i)
		}

		size_t oldSize = _size;
		dispose();
		_size = oldSize;

		data = new_data;
		capacity = new_size;
	}

	void tryToReleaseMemory(){
		if (_size < capacity / 2) {
			resize(capacity / 2);
		}
	}

	void destructContainedObjects() {
		for (size_t i = 0; i < _size; i++) {
			data[i].~T();
		}
	}

public:

	static const size_t npos = -1;

	Vector() {
		_size = capacity = 0;
		data = nullptr;
	}

	Vector(size_t default_size, T default_value) : Vector(default_size) {
		fill(default_value);
	}

	Vector(size_t default_size) : Vector() {
		reserve(default_size);
	}

	Vector(const std::initializer_list<T> &arg_list) : Vector(arg_list.size()) {
		for (auto x : arg_list) {
			push_back(x);
		}
	}

	Vector(const Vector &other) : Vector() {
		reserve(other._size);
		for (size_t i = 0; i < other._size; i++) {
			push_back(other[i]);
		}
	}

	~Vector() {
		dispose();
	}

	void push_back(const T &value) {
		if (capacity == _size) {
			reserve(capacity ? min(capacity * 2, max_size) : 1);
		}
		if (_size >= capacity) {
			throw std::overflow_error("Max capacity exceeded.");
		}
		new (data + _size) T(value); //construct new object at (data+_size)
		_size++;
	}

	void removeAt(size_t index) {
		if (index >= _size)
			return;

		data[index].~T();
		std::copy(data + index + 1, data + _size, data + index);
		_size--;

		tryToReleaseMemory();
	}

	void remove(std::function<bool(T)> predicate) {
		for (size_t i = 0; i < _size; i++) {
			if (predicate(data[i])) {
				removeAt(i);
				return;
			}
		}
	}

	size_t lastIndexOf(std::function<bool(T)> predicate) {
		size_t resultIndex = npos;
		for (size_t i = 0; i < _size; i++) {
			if (predicate(data[i])) {
				resultIndex = i;
			}
		}
		return resultIndex;
	}

	void reserve(size_t new_size) {
		if (new_size <= capacity)
			return;
		if (new_size > max_size) {
			throw std::invalid_argument("Max capacity exceeded");
		}

		resize(new_size);
	}

	size_t size() const {
		return _size;
	}

	void pop_back() {
		if (_size == 0)
			return;

		data[_size-1].~T();
		_size--;

		tryToReleaseMemory();
	}

	void fill(T value) {
		for (size_t i = 0; i < capacity; i++) {
			push_back(value);
		}
	}

	T& find(std::function<bool(T)> predicate) {
		for (size_t i = 0; i < _size; i++) {
			if (predicate(data[i])) {
				return data[i];
			}
		}
		throw std::invalid_argument("Element not found.");
	}

	size_t findIndex(std::function<bool(T)> predicate) {
		for (size_t i = 0; i < _size; i++) {
			if (predicate(data[i])) {
				return i;
			}
		}
		return npos;
	}
	
	void clear() { // destructs contained objects, but does not release memory
		destructContainedObjects();
		_size = 0;
	}

	void dispose() { // destructs contained objects and release memory
		destructContainedObjects();
		operator delete[](data);
		data = nullptr;
		_size = capacity = 0;
	}

	T& operator[](size_t index) const{
		if (index >= _size)
			throw std::invalid_argument("Index does not exists");
		return data[index];
	}

	Vector operator=(const Vector& other) {
		clear();
		for(size_t i = 0; i < other._size; i++) {
			push_back(other[i]);
		}
		return *this;
	}

	T* begin() {
		return data;
	}

	T* end() {
		return data + _size;
	}

};
