#pragma once

#include "ComplexReader.h"

using namespace std;

Complex ComplexReader::parseComplex(const string& complex) { return parseComplex(complex, 0, complex.size()); }

Complex ComplexReader::parseComplex(const string& complex, size_t start) { return parseComplex(complex, start, complex.size()); }

Complex ComplexReader::parseComplex(const string& complex, size_t start, size_t end) { //Complex is always in 'a+bi' format, even if a or b equals to 0 it's gonna be '0+0i'
	end = min(end, complex.size());

	size_t plusIndex = complex.find('+', start);

	string realPart = "", imPart = "";

	for (size_t i = start; i < plusIndex; i++) {
		realPart += complex[i];
	}
	plusIndex = max(plusIndex, start);

	for (size_t i = plusIndex; i < end; i++) {
		imPart += complex[i];
	}

	return Complex(stod(realPart), stod(imPart));
}

bool ComplexReader::isFileInCorrectFormat() {
	try { //getAsArray will parse file content into a Complex's objects, if an error occured while parsing then file is in incorrect format
		vector<Complex> tmpStorage(size());
		getAsArray(tmpStorage.data());
	}
	catch (std::exception e) {
		return false;
	}
	return true;
}

ComplexReader::ComplexReader() :File(), _size(0) {}

ComplexReader::ComplexReader(string filename) :File(filename), _size(0) { open(filename); }

void ComplexReader::open(string filename) {
	File::open(filename);

	string fileContent = readAll();
	_size = count(fileContent.begin(), fileContent.end(), '\n'); //new line means new object, so amount of objects equals amount of '\n' at file

	if (!isFileInCorrectFormat()) {
		throw(std::exception("File is in incorrect format."));
	}
}

void ComplexReader::getAsArray(Complex* destination) {
	string fileContent = readAll();

	size_t itemsIndex = 0, linesIndex = 0, newLineIndex = 0;

	while ((newLineIndex = fileContent.find('\n', linesIndex)) != string::npos) {
		destination[itemsIndex++] = parseComplex(fileContent, linesIndex, newLineIndex);
		linesIndex = newLineIndex + 1;
	}

}

void ComplexReader::writeArray(Complex* source, size_t size) {
	_size = 0;
	clear();
	appendArray(source, size);
}

void ComplexReader::appendArray(Complex* source, size_t size) {
	stringstream complexString;

	for (size_t i = 0; i < size; i++) {
		complexString << source[i] << "\n";
	}

	append(complexString.str());

	_size += size;
}

size_t ComplexReader::size() {
	return _size;
}




