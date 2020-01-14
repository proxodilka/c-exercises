#pragma once
#include "TypeRebinder.h"
#include "../Complex/Complex.h"
#include "ContainerTraits.h"
#include <chrono>

//This class provides you an ability to test your container.
//It obtains a container type as a template argument and rebind contained type to class Complex.
//
//Class provides functional to test some main methods of sequence containers, note that all container
//methods will be invoked through 'ContainerTraits', see ContainerTraits.h for details.
//
//Usage examplex:
//	
//	int main() {
//		ContainerTester< std::vector<int> > tester;
//
//		tester.fill_test();
//		tester.pop_back_test();
//		tester.pop_front_test();
//
//		tester.auto_test();
//		return 0;
//	}


//TODO:
//	1. find a way to reduce amount of same code in test methods

template<class container_type>
class ContainerTester {

	typedef typename rebind_container_type<container_type, Complex>::cont complex_container;
	typedef typename rebind_container_type<container_type, complex_container>::cont nested_container;

	nested_container main_container;
	complex_container compl_container;
	std::ostream& out = std::cout;

	static std::chrono::milliseconds get_time() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	}

	template<class func, typename... Args>
	static double calc_time(func fn, Args&&... args) {
		auto start_time = get_time();

		fn(std::forward<Args>(args)...);

		auto end_time = get_time();
		return std::chrono::duration<double>(end_time - start_time).count();
	}

	template<class func, typename... Args>
	bool execute(func fn, Args&&... args) {
		bool is_success = true;
		try {
			fn(std::forward<Args>(args)...);
		}
		catch (not_supported_operation & e) {
			out << "Exception has been thrown: " << e.what() << "\nOperation: " << e.operation_type() << "\n";
			is_success = false;
		}
		catch (std::exception & e) {
			out << "Exception has been thrown: " << e.what() << "\n";
			is_success = false;
		}
		return is_success;
	}

	template<typename cont_type, typename src>
	void fill_container(cont_type& container, src source, size_t amount) {
		for (size_t i = 0; i < amount; i++) {
			ContainerTraits<cont_type>::push_back(container, source[i]);
		}
	}

public:

	void auto_test() {
		out << "\n============AUTO TEST FOR " << typeid(complex_container).name() << " ================\n";
		fill_test();
		pop_back_test();
		pop_front_test();
	}

	void fill_test(size_t amount = 1e6) {
		out << "\n=============FILL TEST===============\n";
		out << "Amount: " << amount << "\n";
		Complex* model = new Complex[amount];
		double duration = 0; bool is_ok = true;
		size_t error_index = -1;

		for (size_t i = 0; i < amount; i++) {
			model[i] = Complex(rand(), rand());
		}

		bool is_except = execute([&]() {
			ContainerTraits<complex_container>::clear(compl_container);
			duration = calc_time([&](auto&... args) { fill_container(args...); }, compl_container, model, amount);


			size_t index = 0;
			for (auto& value : compl_container) {
				if (value != model[index]) {
					is_ok = false;
					error_index = index;
					break;
				}
				index++;
			}
			if (index != amount) {
				is_ok = false;
			}
		});

		is_ok = is_ok && is_except;

		out << "Results:\n" << "\tDuration: " << duration << "s. \n" << "\tIS PASSED: " << is_ok << "\n";
		if (error_index != -1) {
			out << "\tError occured at index: " << error_index << ".\n\t\tExpected: " << model[error_index] << "\n\t\tGot: " << ContainerTraits<complex_container>::operator_at(compl_container, error_index) << "\nEnd of fill test.\n";
		}

		delete[] model;
	}

	void pop_back_test(size_t amount = 1e6) {
		out << "\n=============POP BACK TEST===============\n";
		out << "Amount: " << amount << "\n";
		Complex* model = new Complex[amount];
		double duration = 0; bool is_ok = true;
		size_t error_index = -1;
		size_t index = 0;

		for (size_t i = 0; i < amount; i++) {
			model[i] = Complex(rand(), rand());
		}

		bool is_except = execute([&]() {
			ContainerTraits<complex_container>::clear(compl_container);
			fill_container(compl_container, model, amount);

			duration = calc_time([&]() {
				for (size_t i = 0; i < amount / 2; i++) {
					ContainerTraits<complex_container>::pop_back(compl_container);
				}
			});

			index = 0;
			for (auto& value : compl_container) {
				if (value != model[index]) {
					is_ok = false;
					error_index = index;
					break;
				}
				index++;
			}
			if (index != amount / 2) {
				is_ok = false;
			}
		});

		is_ok = is_ok && is_except;

		out << "Results:\n" << "\tDuration: " << duration << "s. \n" << "\tIS PASSED: " << is_ok << "\n";
		out << "\tContainer size:\n\t\tExcpected: " << amount / 2 << "\n\t\tActually: " << index << "\n";
		if (error_index != -1) {
			out << "\tError occured at index: " << error_index << ".\n\t\tExpected: " << model[error_index] << "\n\t\tGot: " << ContainerTraits<complex_container>::operator_at(compl_container, error_index) << "\nEnd of fill test.\n";
		}

		delete[] model;
	}

	void pop_front_test(size_t amount = 1e5) {
		out << "\n=============POP FRONT TEST===============\n";
		out << "Amount: " << amount << "\n";
		Complex* model = new Complex[amount];
		double duration = 0; bool is_ok = true;
		size_t error_index = -1;
		size_t index = 0;

		for (size_t i = 0; i < amount; i++) {
			model[i] = Complex(rand(), rand());
		}

		bool is_except = execute([&]() {
			ContainerTraits<complex_container>::clear(compl_container);
			fill_container(compl_container, model, amount);

			duration = calc_time([&]() {
				for (size_t i = 0; i < amount / 2; i++) {
					ContainerTraits<complex_container>::pop_front(compl_container);
				}
			});

			index = amount / 2;
			for (auto& value : compl_container) {
				if (value != model[index]) {
					is_ok = false;
					error_index = index;
					break;
				}
				index++;
			}
			if (index != amount) {
				is_ok = false;
			}
		});

		is_ok = is_ok && is_except;

		out << "Results:\n" << "\tDuration: " << duration << "s. \n" << "\tIS PASSED: " << is_ok << "\n";
		out << "\tContainer size:\n\t\tExcpected: " << amount - amount / 2 << "\n\t\tActually: " << index - amount/2 << "\n";
		if (error_index != -1) {
			out << "\tError occured at index: " << error_index << ".\n\t\tExpected: " << model[error_index] << "\n\t\tGot: " << ContainerTraits<complex_container>::operator_at(compl_container, error_index) << "\nEnd of fill test.\n";
		}

		delete[] model;
	}

};