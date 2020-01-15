#pragma once


class not_implemented_exception : public std::exception
{
public:
	virtual char const* what() const { return "Function not yet implemented."; }
};

template<typename T, class allocator_type = std::allocator<T>>
class HashedArrayTree {
	T** roots;
	size_t _size, arr_capacity_base, roots_size, arr_capacity;

	typename std::allocator_traits<allocator_type>::template rebind_alloc<T*> roots_alloc;
	allocator_type element_alloc;

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
			element_alloc.destroy(this->getPointerByIndex(i));
		}
	}

	T* getPointerByIndex(size_t index) {
		size_t root_n = index / arr_capacity;
		size_t pos_n = index % arr_capacity;
		return (roots[root_n] + pos_n);
	}

	void force_dispose() {
		destructContainedObjects();
		for (size_t i = 0; i < roots_size; i++) {
			element_alloc.deallocate(roots[i], arr_capacity);
		}
		roots_alloc.deallocate(roots, arr_capacity);
		roots = nullptr;

		set_capacity_base(1);
		_size = 0;
		roots_size = 0;
	}

	void init_roots() {
		roots = new T * [arr_capacity];
	}

	void resize(size_t capacity_base) {
		size_t new_arr_capacity = 1 << capacity_base, new_capacity = 1 << (2*capacity_base);
		T** new_roots = roots_alloc.allocate(new_arr_capacity);
		size_t new_root_ind = 0, new_pos_ind = new_arr_capacity;

		for (size_t i = 0; i < min( new_capacity, this->size() ); i++, new_pos_ind++) {
			if (new_pos_ind == new_arr_capacity) {
				new_roots[new_root_ind++] = element_alloc.allocate(new_arr_capacity);
				new_pos_ind = 0;
			}
			element_alloc.construct(new_roots[new_root_ind - 1] + new_pos_ind, std::move((*this)[i]));
		}

		size_t old_size = _size;
		force_dispose();

		roots = new_roots;
		set_capacity_base(capacity_base);
		_size = min(new_capacity, old_size);
		roots_size = _size / new_arr_capacity;
		
	}

	void push_new_leaf() {
		roots[roots_size] = element_alloc.allocate(arr_capacity);
		roots_size++;
	}

	void remove_last_leaf() {
		if (roots_size <= 0) {
			return;
		}
		element_alloc.deallocate(roots[roots_size - 1], arr_capacity);
		roots_size--;
	}

	void try_to_release_memory() {
		size_t pos_n = (_size) % arr_capacity;

		if (pos_n == 0) {
			remove_last_leaf();
		}

		if (roots_size < arr_capacity / 4) {
			resize(arr_capacity_base - 1);
		}
	}

	void rvalue_assing(HashedArrayTree&& other) {
		_size = other._size;
		roots = other.roots;
		arr_capacity_base = other.arr_capacity_base;
		arr_capacity = other.arr_capacity;
		roots_size = other.roots_size;

		other.roots = nullptr;
		other._size = 0;
		other.arr_capacity = 0;
		other.arr_capacity_base = 0;
		other.roots_size = 0;
	}

public:

	class iterator;
	class const_iterator;

	friend iterator;
	friend const_iterator;

	HashedArrayTree(allocator_type allocator = allocator_type()): element_alloc(allocator){
		
		_size = 0;
		set_capacity_base(1);
		roots_size = 0;
		init_roots();
	}

	HashedArrayTree(size_t default_size) :HashedArrayTree() {
		reserve(default_size);
	}

	HashedArrayTree(std::initializer_list<T> values) :HashedArrayTree(values.size()) {
		for (auto& value : values) {
			push_back(value);
		}
	}

	HashedArrayTree(const HashedArrayTree& other) :HashedArrayTree(other._size) {
		for (auto& value : other) {
			push_back(value);
		}
	}

	HashedArrayTree(HashedArrayTree&& other) {
		rvalue_assing(std::move(other));
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
		resize(log2(log2(amount)) + 0.5);
	}

	void pop_back() {
		if (_size == 0) {
			return;
		}

		(*this)[_size - 1].~T();
		_size--;
		try_to_release_memory();

	}

	void removeAt(size_t index) {
		if (index >= _size) {
			return;
		}

		(*this)[index].~T();
		for (size_t i = index; i < this->_size -1; i++) {
			(*this)[i] = std::move((*this)[i+1]);
		}

		_size--;
		try_to_release_memory();
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
		size_t root_n = index / arr_capacity;
		size_t pos_n = index % arr_capacity;
		return *(roots[root_n] + pos_n);
	}

	const T& operator[](size_t index) const {
		if (index >= _size) {
			throw std::invalid_argument("Index does not exist");
		}
		size_t root_n = index / arr_capacity;
		size_t pos_n = index % arr_capacity;
		return *(roots[root_n] + pos_n);
	}

	HashedArrayTree& operator=(const HashedArrayTree& other) {
		dispose();
		reserve(other._size);
		for (auto& value : other) {
			push_back(value);
		}
		
		return *this;
	}

	HashedArrayTree& operator=(HashedArrayTree&& other) {
		force_dispose();
		rvalue_assing(std::move(other));

		return *this;
	}

	size_t size() const {
		return _size;
	}

	iterator begin() {
		return iterator(0, this);
	}

	iterator end() {
		return iterator(this->size(), this);
	}

	const_iterator begin() const {
		return const_iterator(0, this);
	}

	const_iterator end() const {
		return const_iterator(this->size(), this);
	}

	const_iterator cbegin() {
		return const_iterator(0, this);
	}

	const_iterator cend() {
		return const_iterator(this->size(), this);
	}

};

template<typename T, class allocator_type>
class HashedArrayTree<T, allocator_type>::iterator : public std::iterator<std::random_access_iterator_tag, T> {
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

template<typename T, class allocator_type>
class HashedArrayTree<T, allocator_type>::const_iterator : public iterator {
	friend HashedArrayTree;
	const_iterator(size_t index, const HashedArrayTree* sourceHAT) : iterator(index, const_cast<HashedArrayTree*>(sourceHAT)) {}

public:
	const_iterator(const iterator& other): iterator(other.index, other.sourceHAT) {}

	const T& operator*() { return *(this->pointer); }
	const T& operator[](int offset) { return *(this->sourceHAT->getPointerByIndex(this->index + offset)); }
};

