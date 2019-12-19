#pragma once

#include <fstream>
#include <algorithm>

using namespace std;

class File {

protected:
	string filename;
	fstream file;

public:

	File() {}

	File(string filename) {
		open(filename);
	}

	~File() {
		close();
	}

	virtual void open(string filename) {
		open(filename, false);
	}

	virtual void open(string filename, bool isTruncate) {
		this->filename = filename;

		file.open(filename, fstream::out | fstream::app); //this will create new empty file if it doesn't exist
		close();

		file.open(filename, fstream::in | fstream::out | fstream::binary | (isTruncate ? fstream::trunc : 0));

	}

	void close() {
		file.close();
	}

	void clear() {
		close();
		open(filename, true);
	}

	int length() {
		int old_pos = file.tellg();

		file.seekg(0, fstream::end);
		int length = file.tellg();

		file.seekg(old_pos);
		return length;
	}

	int getPosition() {
		return file.tellg();
	}

	void seek(int position) {
		file.seekp(position);
		file.seekg(position);
	}

	void read(char* dest, unsigned int lenghtOfReadableContent) {
		int symbolsToEnd = length() - file.tellg();
		file.seekg(file.tellg());
		file.read(dest, min((int)lenghtOfReadableContent, symbolsToEnd));
		dest[dest, min((int)lenghtOfReadableContent, symbolsToEnd)] = '\0';
	}

	string read(unsigned int lenghtOfReadableContent) {

		char* tmp_dest = new char[lenghtOfReadableContent + 1];
		read(tmp_dest, lenghtOfReadableContent);

		string result(tmp_dest);

		delete[] tmp_dest;
		return result;
	}

	void read(char* dest) { read(dest, length()); }
	string read() { return read(length()); }
	string readAll() {
		seek(0);
		return read(length());
	}

	void write(string content) {
		file.seekp(file.tellp());
		file.write(content.c_str(), content.size());
		file.flush();
	}

	bool is_open() {
		return file.is_open();
	}

	void append(string content) {
		seek(length());
		write(content);
	}

};