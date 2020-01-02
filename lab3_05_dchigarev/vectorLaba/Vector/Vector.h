#pragma once
#include <functional>

#define min(a,b) (((a)<(b))?(a):(b))

template<typename T, class allocator_type = std::allocator<T>>
class Vector {
private:
	const static size_t max_size = INT_MAX;
	size_t _size, capacity;
	T* _data;
	allocator_type allocator;

	void resize(size_t new_size) { 

		T* new_data = allocator.allocate(new_size);
		for (size_t i = 0; i < min(_size, new_size); i++) {
			std::allocator_traits<allocator_type>::construct(allocator, new_data + i, _data[i]);
		}

		size_t oldSize = _size;
		dispose();
		_size = oldSize;

		_data = new_data;
		capacity = new_size;
	}

	void tryToReleaseMemory() {
		if (_size < capacity / 4) {
			resize(capacity / 2);
		}
	}

	void destructContainedObjects() {
		for (size_t i = 0; i < _size; i++) {
			std::allocator_traits<allocator_type>::destroy(allocator, _data + i);
		}
	}

public:

	static const size_t npos = -1;
	class iterator;
	class const_iterator;

	Vector(const allocator_type& allocator = allocator_type()) {
		_size = capacity = 0;
		_data = nullptr;
		this->allocator = allocator;
	}

	Vector(size_t default_size, const allocator_type& allocator = allocator_type()) : Vector(allocator) {
		reserve(default_size);
	}

	Vector(size_t default_size, T default_value, const allocator_type& allocator = allocator_type()) : Vector(default_size, allocator) {
		fill(default_value);
	}

	Vector(const std::initializer_list<T> &arg_list) : Vector(arg_list.size()) {
		for (auto x : arg_list) {
			push_back(x);
		}
	}

	Vector(const Vector &other) : Vector(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.allocator)) {
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
		std::allocator_traits<allocator_type>::construct(allocator, _data + _size, value);
		_size++;
	}

	void removeAt(size_t index) {
		if (index >= _size)
			return;

		std::allocator_traits<allocator_type>::destroy(allocator, _data + index);
		std::copy(_data + index + 1, _data + _size, _data + index);
		_size--;

		tryToReleaseMemory();
	}

	void remove(std::function<bool(const T&)> predicate) {
		for (size_t i = 0; i < _size; i++) {
			if (predicate(_data[i])) {
				removeAt(i);
				return;
			}
		}
	}

	size_t lastIndexOf(std::function<bool(const T&)> predicate) const{
		size_t resultIndex = npos;
		for (size_t i = 0; i < _size; i++) {
			if (predicate(_data[i])) {
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

	size_t size() const noexcept{
		return _size;
	}

	void pop_back() {
		if (_size == 0)
			return;

		std::allocator_traits<allocator_type>::destroy(allocator, _data + _size-1);
		_size--;

		tryToReleaseMemory();
	}

	void fill(const T& value) {
		for (size_t i = _size; i < capacity; i++) {
			push_back(value);
		}
	}

	T& find(std::function<bool(const T&)> predicate) {
		for (size_t i = 0; i < _size; i++) {
			if (predicate(_data[i])) {
				return _data[i];
			}
		}
		throw std::invalid_argument("Element not found.");
	}

	size_t findIndex(std::function<bool(const T&)> predicate) const{
		for (size_t i = 0; i < _size; i++) {
			if (predicate(_data[i])) {
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
		allocator.deallocate(_data, capacity);
		_data = nullptr;
		_size = capacity = 0;
	}

	T& operator[](size_t index) {
		if (index >= _size)
			throw std::invalid_argument("Index does not exists");
		return _data[index];
	}

	Vector& operator=(const Vector& other) {
		clear();
		reserve(other.size());
		for(size_t i = 0; i < other._size; i++) {
			push_back(other[i]);
		}
		return *this;
	}

	iterator begin() noexcept{
		return iterator(_data);
	}

	iterator end() noexcept{
		return iterator(_data + _size);
	}

	const_iterator cbegin() const noexcept{
		return const_iterator(_data);
	}

	const_iterator cend() const noexcept{
		return const_iterator(_data + _size);
	}

	T* data() noexcept{
		return _data;
	}
};


template<typename T, class allocator_type>
class Vector<T, allocator_type>::iterator : public std::iterator<std::random_access_iterator_tag, T> {
	protected:
		friend Vector;
		T* pointer;
		iterator(T* pointer) noexcept : pointer(pointer) {}
	public:
		T& operator*() { return *(this->pointer); }
		T& operator[](int offset) { return *(this->pointer + offset); }

		iterator operator+(int value) { return iterator(this->pointer + value); }
		iterator operator-(int value) { return iterator(this->pointer - value); }

		bool operator==(const iterator& it) { return it.pointer == this->pointer; }
		bool operator!=(const iterator& it) { return it.pointer != this->pointer; }
		bool operator<(const iterator& it) { return this->pointer < it.pointer; }
		bool operator>(const iterator& it) { return this->pointer > it.pointer; }
		bool operator<=(const iterator& it) { return this->pointer <= it.pointer; }
		bool operator>=(const iterator& it) { return this->pointer >= it.pointer; }

		iterator& operator++(int) { this->pointer++; return *this; }
		iterator& operator--(int) { this->pointer--; return *this; }
		iterator operator++() {
			iterator retValue(*this);
			this->pointer++;
			return retValue;
		}
		iterator operator--() {
			iterator retValue(*this);
			this->pointer--;
			return retValue;
		}
		iterator& operator+=(int value) { this->pointer += value; return *this; }
		iterator& operator-=(int value) { this->pointer -= value; return *this; }
};

template<typename T, class allocator_type>
class Vector<T, allocator_type>::const_iterator : iterator {
	friend Vector;
public:
	const_iterator(const iterator& it) noexcept : iterator(it.pointer) {}

	const T& operator*() const { return *(this->pointer); }
	const T& operator[](int offset) const { return *(this->pointer + offset); }
};
