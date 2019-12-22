#pragma once
#include <ostream>

class Complex {
private:
	double im, re;
public:

	Complex();
	Complex(double re, double im);

	friend std::ostream& operator<<(std::ostream& out, const Complex& data);
};
