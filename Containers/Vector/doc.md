***Vector is sequence a container, that represents dynamic array, which grows in size with adding new elements.***


# Constants:
**Vector::npos**

`static const size_t npos = -1`

Max possible value for size_t. *As a return value means invalid index.*


# Methods:

-----------------------------------
### Vector::Vector<T, allocator_type>
```
default             (1) | Vector(allocator_type& allocator = allocator_type())
reserving           (2) | Vector(size_t default_size, allocator_type& allocator = allocator_type())
fill                (3) | Vector(size_t default_size, T default_value, allocator_type& allocator = allocator_type())
copy constr.        (4) | Vector(const Vector &other)
rvalue copy constr. (5) | Vector(Vector&& other)
init list           (6) | Vector(const std::initializer_list<T> &arg_list)
```
**Overloads:**

- (1) Constructs an empty Vector
- (2) Constructs an empty Vector and 'reserves' memory for 'default_size' amount of objects
- (3) Constructs Vector and fill it with 'default_size' copies of 'default_value' objects
- (4) Copy constructor, copy all objects from 'other' to new created Vector
- (5) Creates copy from rvalue reference.
- (6) Constructs Vector and copy all objects from 'arg_list' to new created Vector

**Template parameters:**
- T - type of stored elements
- allotator_type - type of allocator, std::allocator<T> by default


**Complexity:**

Constant for (1), (2) and (5), in other cases linear of new created Vector size

**Exceptions:**

(1) and (4) are not throws exceptions, in other constructors `std::invalid_argument`
may be thrown from `Vector::reserve` method if the wished size of new Vector is greater than `Vector::max_size`

-----------------------------------
### Vector::~Vector

Calls `Vector::dispose` which destructs all contained objects and release allocated memory

**Complexity:**

Linear in `Vector::size`

-----------------------------------
### Vector::begin
```
iterator begin()
```
Allows you to get iterator.

**Return value:**

- `Vector::iterator` to the first element in container, equals to `Vector::end()` if container is empty

**Complexity:**
  
   - Constant

**Exceptions:**

- Never throws exceptions
  
-----------------------------------
### Vector::cbegin
```
const_iterator cbegin()
```
Allows you to get constant iterator.

**Return value:**

- `Vector::contant_iterator` to the first element in container, equals to `Vector::cend()` if container is empty

**Complexity:**
  
 - Constant

**Exceptions:**

- Never throws exceptions

-----------------------------------
### Vector::cend
```
const_iterator end()
```
Allows you to get constant iterator to the next after the last element in Vector.
Should not be dereferencing.

**Return value:**
- contant iterator to the next after the last element, if Vector is empty returns the same as 'Vector::cbegin'

**Complexity:**
- Constant

**Exceptions:**
- Never throws exceptions

-----------------------------------
### Vector::clear
```
void clear()
```
Destructs contained objects, and set `Vector::size` to zero. Does not release allocated memory, so calling `Vector::push_back`
for 'old size' times will not cause reallocation. To clear Vector with releasing memory use `Vector::dispose` instead

**Return value:**
	
- void

**Complexity:**

- Linear in `Vector::size`

**Exceptions:**
	
- Never throws exceptions

-----------------------------------
### Vector::dispose
```
void dispose()
```
Destructs contained objects, and set `Vector::size` to zero. Also releases alloccated memory.

**Return value:**
- void

**Complexity:**
- Linear in `Vector::size`

**Exceptions:**

- Never throws exceptions

-----------------------------------
### Vector::end
```
iterator end()
```
Allows you to get iterator to the next after the last element in Vector.
Should not be dereferencing.

**Return value:**
- iterator to the next after the last element, if Vector is empty returns the same as 'Vector::begin'

**Complexity:**
- Constant

**Exceptions:**
- Never throws exceptions

-----------------------------------
### Vector::fill
```
void fill(T value)
```
Calls `push_back(value)` for `capacity - size` times. Fills all free space with `value`

**Arguments:**
- `T value` - object with which fill Vector

**Return value:**

- void

**Complexity:**

- Linear in `Vector::capacity - Vector::size`

**Exceptions:**

- Never throws exceptions

-----------------------------------
### Vector::find
```
T& find(std::function<bool(const T&)> predicate)
```
Searches the first element in Vector that satisfies `predicate` function

**Arguments:**

- `std::function<bool(const T&)> predicate` - function that takes reference to the T object and returns true if this element satisfies required conditions.

**Return value:**

- Reference to the find element

**Complexity:**

- Linear in `Vector::size`

**Exceptions:**

-	Throws `std::invalid_argument` if element not found

-----------------------------------
### Vector::findIndex
```
size_t findIndex(std::function<bool(const T&)> predicate)
```
Searches the first element in Vector that satisfies `predicate` function

**Arguments:**

- `std::function<bool(const T&)> predicate` - function that takes reference to the T object and returns true if this element satisfies required conditions.

**Return value:**
- Index of find element, if it is not found returns `Vector::npos`

**Complexity:**
- Linear in `Vector::size`

**Exceptions:**

-	Never throws exceptions


-----------------------------------
### Vector::lastIndexOf
```
size_t lastIndexOf(std::function<bool(const T&)> predicate)
```
Searches the last element in Vector that satisfies `predicate` function

**Arguments:**

- `std::function<bool(const T&)> predicate` - function that takes reference to the T object and returns true if this element satisfies required conditions.

**Return value:**
- Index of last element that satisfice conditions, if it is not found returns `Vector::npos`

**Complexity:**
- Linear in `Vector::size`

**Exceptions:**

-	Never throws exceptions

-----------------------------------
### Vector::pop_back
```
void pop_back()
```
Desctructs last element in Vector and removes it, if Vector is empty does nothing.
	
*! This method calls 'Vector::tryToReleaseMemory' so it may invoke reallocation !*

**Return value:**

- void

**Complexity:**

- Constant. If reallocation occured linear in `Vector::size`

**Exceptions:**

- Never throws exceptions

-----------------------------------
### Vector::push_back
```
void push_back(const T &value)
void push_back(T&& value)
```
Adds copy of `value` to the end of Vector.

*! This method may invoke reallocation if new Vector size exceeds current capacity, to avoid it use 'Vector::reserve' !*

**Arguments:**

- `const T &value` - reference to the object to push
- `T&& value` - rvalue reference to the object, in this case object will be placed through `std::move`

**Return value:**

- void

**Complexity:**

- Constant. If reallocation occured linear in `Vector::size`

**Exceptions:**

- Throws `std::overflow_error` if new Vector size exceeds `Vector::max_size`

-----------------------------------
### Vector::remove
```
void remove(std::function<bool(const T&)> predicate)
```
Destructs and removes first element in Vector that satisfies predicate function, if element not found does nothing

*! This method calls 'Vector::tryToReleaseMemory' so it may invoke reallocation !*

**Arguments:**

- `std::function<bool(const T&)> predicate` - function that takes reference to the T object and returns true if this element satisfies required conditions and should be removed.

**Return value:**

- void

**Complexity:**

- Constant. If reallocation occured linear in `Vector::size`

**Exceptions:**

- Never throws exceptions

-----------------------------------
### Vector::removeAt
```
void removeAt(size_t index)
```
Destructs and removes element in Vector at given 'index'. If 'index' is greater than 'Vector::size' does nothing

*! This method calls 'Vector::tryToReleaseMemory' so it may invoke reallocation !*

**Arguments:**

- `size_t index` - index of element to remove
	
**Return value:**

- void

**Complexity:**

- Linear in 'Vector::size'

**Exceptions:**

- Never throws exceptions

-----------------------------------
### Vector::reserve
```
void reserve(size_t new_size)
```
Allocates memory for `new_size` of `T` objects. If `new_size` is smaller than `Vector::capacity` does nothing

**Arguments:**

- `size_t new_size` - amount of 'T' objects for what allocation requires, should not be greater `Vector::max_size`

**Return value:**

- void

**Complexity:**

- Linear in `Vector::size`

**Exceptions:**
	
 - Throws `std::invalid_argument` if `new_size` is greater than `Vector::max_size`

-----------------------------------
### Vector::size
```
size_t size()
```
Returns amount of contained elements in Vector

**Return value:**

- Amount of elements in Vector

**Complexity:**

- Constant.

**Exceptions:**

- Never throws exceptions


# Operators:

-----------------------------------
### Vector::operator[]
```
T& operator[](size_t index) const
```
Allows you to get access to the Vector element by given index

**Return value:**

- Link to the element locating at given index

**Complexity:**

- Constant.

**Exceptions:**

- Throws `std::invalid_argument` if index is greater or equals `Vector::size`

-----------------------------------
### Vector::operator=
```
Vector& operator=(const Vector &other)
Vector& operator=(Vector&& other)
```
Assigns 'other' Vector content to current.

**Arguments:**

- `const Vector &other` - reference to Vector that need to be assigned
- `Vector&& other` - rvalue reference to other Vector

**Return value:**

- Reference to current Vector to allow multi-assignment

**Complexity:**

- Linear in `other` size

**Exceptions:**

- Never throws exceptions




# Implementation details:

Vector is a wrapper of array, which stores all Vector elements, it's represents by private field `T* data`. Vector has private fields `_size` and `capacity`,
first represents the amount of stored objects and second - actual amount of objects that can fit in allocated memory.

If after new insertion `_size` becames greater or equals to `capacity` Vector allocate memory for `min(max_size, capacity * 2)` elements,
that allows to achive amortized O(1) complexity of adding elements. `max_size` is private static constant that represent max Vector
capacity and equals to `INT_MAX`.

Some methods calls `Vector::tryToReleaseMemory` it's invokes reallocation to `capacity / 2` if `_size < capacity / 2`, it's allows us to optimize memory ussage

Reallocation represents by `resize` private method, it takes `new_size` argument and allocates memory for that amount of T elements and 'moves' stored
objects in there, then it releases memory for old array.

	
	
	
