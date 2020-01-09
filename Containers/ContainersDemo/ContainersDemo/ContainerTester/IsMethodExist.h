#pragma once

template<typename T, typename... Args>
struct has_push_back {
private:

	template<typename ...>
	static char f(...) { return 0; }

	template<typename C, typename... Margs>
	static decltype(std::declval<C>().push_back(std::declval<Margs>()...), int()) f(int x) { return 0; }

public:
	static const bool value = sizeof(f<T, Args...>(2)) == sizeof(int);
};

template<typename T, typename... Args>
struct has_push {
private:

	template<typename ...>
	static char f(...) { return 0; }

	template<typename C, typename... Margs>
	static decltype(std::declval<C>().push(std::declval<Margs>()...), int()) f(int x) { return 0; }

public:
	static const bool value = sizeof(f<T, Args...>(2)) == sizeof(int);
};

template<typename T, typename... Args>
struct has_pop_back {
private:

	template<typename ...>
	static char f(...) { return 0; }

	template<typename C, typename... Margs>
	static decltype(std::declval<C>().pop_back(std::declval<Margs>()...), int()) f(int x) { return 0; }

public:
	static const bool value = sizeof(f<T, Args...>(2)) == sizeof(int);
};

template<typename T, typename... Args>
struct has_pop_front {
private:

	template<typename ...>
	static char f(...) { return 0; }

	template<typename C, typename... Margs>
	static decltype(std::declval<C>().pop_front(std::declval<Margs>()...), int()) f(int x) { return 0; }

public:
	static const bool value = sizeof(f<T, Args...>(2)) == sizeof(int);
};

template<typename T, typename... Args>
struct has_pop {
private:

	template<typename ...>
	static char f(...) { return 0; }

	template<typename C, typename... Margs>
	static decltype(std::declval<C>().pop(std::declval<Margs>()...), int()) f(int x) { return 0; }

public:
	static const bool value = sizeof(f<T, Args...>(2)) == sizeof(int);
};

template<typename T, typename... Args>
struct has_operator_at {
private:

	template<typename ...>
	static char f(...) { return 0; }

	template<typename C, typename... Margs>
	static decltype(std::declval<C>().operator[](std::declval<Margs>()...), int()) f(int x) { return 0; }

public:
	static const bool value = sizeof(f<T, Args...>(2)) == sizeof(int);
};

template<typename T, typename... Args>
struct has_push_front {
private:

	template<typename ...>
	static char f(...) { return 0; }

	template<typename C, typename... Margs>
	static decltype(std::declval<C>().push_front(std::declval<Margs>()...), int()) f(int x) { return 0; }

public:
	static const bool value = sizeof(f<T, Args...>(2)) == sizeof(int);
};

template<typename T, typename... Args>
struct has_removeAt {
private:

	template<typename ...>
	static char f(...) { return 0; }

	template<typename C, typename... Margs>
	static decltype(std::declval<C>().removeAt(std::declval<Margs>()...), int()) f(int x) { return 0; }

public:
	static const bool value = sizeof(f<T, Args...>(2)) == sizeof(int);
};

template<typename T, typename... Args>
struct has_erase {
private:

	template<typename ...>
	static char f(...) { return 0; }

	template<typename C, typename... Margs>
	static decltype(std::declval<C>().erase(std::declval<Margs>()...), int()) f(int x) { return 0; }

public:
	static const bool value = sizeof(f<T, Args...>(2)) == sizeof(int);
};

template<typename T, typename... Args>
struct has_clear {
private:

	template<typename ...>
	static char f(...) { return 0; }

	template<typename C, typename... Margs>
	static decltype(std::declval<C>().clear(std::declval<Margs>()...), int()) f(int x) { return 0; }

public:
	static const bool value = sizeof(f<T, Args...>(2)) == sizeof(int);
};

template<typename T, typename... Args>
struct has_insert {
private:

	template<typename ...>
	static char f(...) { return 0; }

	template<typename C, typename... Margs>
	static decltype(std::declval<C>().insert(std::declval<Margs>()...), int()) f(int x) { return 0; }

public:
	static const bool value = sizeof(f<T, Args...>(2)) == sizeof(int);
};
