#pragma once
#include <ostream>

class Complex {
private:
	double im, re;
public:

	Complex();
	Complex(double re, double im);

	bool operator==(const Complex& other);
	bool operator!=(const Complex& other);

	friend std::ostream& operator<<(std::ostream& out, const Complex& data);
};
