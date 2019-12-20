#pragma once

#include "../File/File.h"
#include "../Complex/Complex.h"
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>


class ComplexReader : public File {
private:
	size_t _size;

	static Complex parseComplex(const std::string& complex);

	static Complex parseComplex(const std::string& complex, size_t start);

	static Complex parseComplex(const std::string& complex, size_t start, size_t end);

	bool isFileInCorrectFormat();

public:
	ComplexReader();

	ComplexReader(std::string filename);

	void open(std::string filename);

	void getAsArray(Complex* destination);

	void writeArray(Complex* source, size_t size);

	void appendArray(Complex* source, size_t size);

	size_t size();

};


