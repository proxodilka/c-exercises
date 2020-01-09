#pragma once
#include "Complex.h"

using namespace std;

Complex::Complex() :im(0), re(0) {};
Complex::Complex(double re, double im) :re(re), im(im) {};

bool Complex::operator==(const Complex& other) { return im == other.im && re == other.re; }
bool Complex::operator!=(const Complex& other) { return !((*this) == other); }

ostream& operator<<(ostream& out, const Complex& data) {
	out << data.re << "+" << data.im << "i";
	return out;
}
