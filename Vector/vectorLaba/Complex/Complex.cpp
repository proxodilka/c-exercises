#pragma once
#include "Complex.h"

using namespace std;

Complex::Complex() :im(0), re(0) {};
Complex::Complex(double re, double im) :re(re), im(im) {};

ostream& operator<<(ostream& out, const Complex& data) {
	out << data.re << "+" << data.im << "i";
	return out;
}
