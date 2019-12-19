#pragma once

#include "File.h"
#include "Complex.h"
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class ComplexReader : public File {
private:
	int _size;

	static Complex parseComplex(const string& complex) { return parseComplex(complex, 0, complex.size()); }
	static Complex parseComplex(const string& complex, int start) { return parseComplex(complex, start, complex.size()); }

	static Complex parseComplex(const string& complex, int start, int end) { //Complex is always in 'a+bi' format, even if a or b equals to 0 it's gonna be '0+0i'
		int plusIndex = complex.find('+', start);

		string realPart = "", imPart = "";

		for (int i = start; i < plusIndex; i++) {
			realPart += complex[i];
		}
		plusIndex = max(plusIndex, start);

		for (int i = plusIndex; i < end; i++) {
			imPart += complex[i];
		}

		return Complex(stod(realPart), stod(imPart));
	}

	bool isFileInCorrectFormat() {
		try { //getAsArray will parse file content into a Complex's objects, if an error occured while parsing then file is in incorrect format
			vector<Complex> tmpStorage(size());
			getAsArray(tmpStorage.data());
		}
		catch (std::exception e) {
			return false;
		}
		return true;
	}

public:
	ComplexReader():File(), _size(0) {}
	ComplexReader(string filename) :File(filename), _size(0) {
		open(filename);
	}

	void open(string filename) {
		File::open(filename);

		string fileContent = readAll();
		_size = count(fileContent.begin(), fileContent.end(), '\n'); //new line means new object, so amount of objects equals amount of '\n' at file

		if (!isFileInCorrectFormat()) {
			throw(std::exception("File is in incorrect format."));
		}
	}

	void getAsArray(Complex* destination) {
		string fileContent = readAll();

		int itemsIndex = 0, linesIndex = 0, newLineIndex = 0;

		while ((newLineIndex = fileContent.find('\n', linesIndex)) != string::npos) {
			destination[itemsIndex++] = parseComplex(fileContent, linesIndex, newLineIndex);
			linesIndex = newLineIndex + 1;
		}



	}

	void writeArray(Complex* source, int size) {
		_size = 0;
		clear();
		appendArray(source, size);
	}

	void appendArray(Complex* source, int size) {
		stringstream complexString;

		for (int i = 0; i < size; i++) {
			complexString << source[i] << "\n";
		}

		append(complexString.str());

		_size += size;
	}

	int size() {
		return _size;
	}

};