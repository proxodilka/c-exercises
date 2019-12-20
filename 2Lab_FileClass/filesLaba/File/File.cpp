#pragma once
#include "File.h"

using namespace std;

File::File() {
	file.exceptions(fstream::failbit);
}

File::File(string filename):File() {
	open(filename);
}

File::~File() {
	close();
}

void File::open(string filename) {
	createFile(filename);
	open(filename, defaultMode);
}

void File::open(string filename, int mode) {
	this->filename = filename;
	this->mode = mode;
	file.open(filename, mode);
}

void File::createFile(string filename) {
	fstream tmpStream(filename, fstream::out | fstream::app);
	tmpStream.close();
}

void File::close() {
	if (file.is_open()) { // if we are trying to close file without opening it before, fstream sets failbit and throws exception, we dont want it
		filename = "";
		file.close();
	}
}

void File::clear() {
	close();
	open(filename, mode | fstream::trunc);
}

int File::length() {
	int oldPos = file.tellg();

	file.seekg(0, fstream::end);
	int length = file.tellg();

	file.seekg(oldPos);
	return length;
}

int File::getPosition() {
	return file.tellg();
}

void File::seek(int position) {
	file.seekp(position);
	file.seekg(position);
}

unsigned int File::read(char* dest, unsigned int lenghtOfReadableContent) {
	unsigned int symbolsToEnd = length() - file.tellg()
			   , actualAmountOfSymbolsToRead = min(lenghtOfReadableContent, symbolsToEnd);

	file.read(dest, actualAmountOfSymbolsToRead);
	dest[dest, actualAmountOfSymbolsToRead] = '\0';

	return actualAmountOfSymbolsToRead;
}

string File::read(unsigned int lenghtOfReadableContent) {

	char* tmpDest = new char[lenghtOfReadableContent + 1];
	read(tmpDest, lenghtOfReadableContent);

	string result(tmpDest);

	delete[] tmpDest;
	return result;
}

unsigned int File::readToEnd(char* dest) { return read(dest, length()); }
string File::readToEnd() { return read(length()); }
string File::readAll() {
	seek(0);
	return read(length());
}

void File::write(string content) {
	file.write(content.c_str(), content.size());
	file.flush();
}

bool File::isOpen() {
	return file.is_open();
}

void File::append(string content) {
	seek(length());
	write(content + "\n");
}




