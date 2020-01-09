#pragma once
#include "IsMethodExist.h"
#include "TypeRebinder.h"

//This class allows you to generalize containers methods invoking.
//Now it has few alias for main containers methods, but you can expand for your purposes similarly.

//Usage example:
//
//	int main() {
//		std::vector<int> v = { 1,2,3,4,5 };
//		ContainerTraits< std::vector<int> >::pop_front(v); //std::vector does not have method 'pop_front', 
//														   //ContainerTraits will replace it with 'erase(v.begin())'
//		return 0;
//	}

struct not_supported_operation : public std::exception {
	char operation_name[64];
	not_supported_operation(const char* op_name) {
		int i = 0;
		for (i = 0; op_name[i] && i<63; i++) {
			operation_name[i] = op_name[i];
		}
		operation_name[i] = '\0';
	}
	virtual const char* what() { return "Container does not support this operation."; }
	const char* operation_type() { return operation_name; }
};

template<typename T>
class ContainerTraits {
public:
	typedef typename rebind_container_type<T, int>::type contained_type;

	//push_back
	template<typename... Args>
	static std::enable_if_t< has_push_back<T, Args...>::value > push_back(T& container, Args&&... args) {
		container.push_back(std::forward<Args>(args)...);
	}

	template<typename... Args>
	static std::enable_if_t< !has_push_back<T, Args...>::value && has_push<T, Args...>::value> push_back(T& container, Args&&... args) {
		container.push(std::forward<Args>(args)...);
	}

	template<typename...>
	static void push_back(...) {
		throw not_supported_operation("push_back");
	}

	//push_front
	template<typename... Args>
	static std::enable_if_t< has_push_front<T, Args...>::value > push_front(T& container, Args&&... args) {
		container.push_front(std::forward<Args>(args)...);
	}

	template<typename... Args>
	static std::enable_if_t< !has_push_front<T, Args...>::value && has_insert<T, typename T::iterator>::value > push_front(T& container, Args&&... args) {
		container.insert(container.begin(), std::forward<Args>(args)...);
	}

	template<typename...>
	static void push_front(...) {
		throw not_supported_operation("push_front");
	}

	//pop_front
	template<typename... Args>
	static std::enable_if_t< has_pop_front<T, Args...>::value > pop_front(T& container, Args&&... args) {
		container.pop_front(std::forward<Args>(args)...);
	}

	template<typename... Args>
	static std::enable_if_t < !has_pop_front<T, Args...>::value && has_pop<T, Args...>::value > pop_front(T& container, Args&&... args) {
		container.pop(std::forward<Args>(args)...);
	}

	template<typename... Args>
	static std::enable_if_t< !has_pop_front<T, Args...>::value && !has_pop<T, Args...>::value && has_removeAt<T, size_t>::value > pop_front(T& container, Args&&... args) {
		container.removeAt(0);
	}

	template<typename... Args>
	static std::enable_if_t< !has_pop_front<T, Args...>::value && !has_pop<T, Args...>::value && !has_removeAt<T, size_t>::value && has_erase<T, typename T::iterator>::value > pop_front(T& container, Args&&... args) {
		container.erase(container.begin());
	}

	template<typename...>
	static void pop_front(...) {
		throw not_supported_operation("pop_front");
	}

	//pop_back
	template<typename... Args>
	static std::enable_if_t< has_pop_back<T, Args...>::value > pop_back(T& container, Args&&... args) {
		container.pop_back(std::forward<Args>(args)...);
	}

	template<typename...>
	static void pop_back(...) {
		throw not_supported_operation("pop_back");
	}

	//clear
	template<typename... Args>
	static std::enable_if_t< has_clear<T, Args...>::value > clear(T& container, Args&&... args) {
		container.clear(std::forward<Args>(args)...);
	}

	template<typename... Args>
	static std::enable_if_t< !has_clear<T, Args...>::value && has_dispose<T, Args...>::value> clear(T& container, Args&&... args) {
		container.dispose(std::forward<Args>(args)...);
	}

	template<typename...>
	static void clear(...) {
		throw not_supported_operation("clear");
	}

	//operator[]
	template<typename... Args>
	static decltype(std::declval<T>().operator[](std::declval<Args>()...), std::declval<contained_type&>()) operator_at(T& container, Args&&... args) {
		return container.operator[](std::forward<Args>(args)...);
	}

	template<typename...>
	static contained_type operator_at(...) {
		throw not_supported_operation("operator[]");
	}

};