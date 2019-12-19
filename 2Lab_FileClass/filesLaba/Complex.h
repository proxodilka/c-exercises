#pragma once
#include <ostream>

class Complex {
public:
	double im, re;

	Complex() :im(0), re(0) {};
	Complex(double re, double im) :re(re), im(im) {};

	friend std::ostream& operator<<(std::ostream& out, Complex& data) {
		out << data.re << "+" << data.im << "i";
		return out;
	}
};