#pragma once

#include <fstream>
#include <algorithm>


class File {

protected:
	std::string filename;
	std::fstream file;
	int defaultMode = fstream::in | fstream::out | fstream::binary,
		mode;

public:

	File();

	File(std::string filename);

	~File();

	virtual void open(std::string filename); //open file with defaultMode, create new file if it doesnt exist

	virtual void open(std::string filename, int mode);

	void createFile(std::string filename); //create new file if it doesnt exist

	void close();

	void clear(); //clear file content

	int length();

	int getPosition();

	void seek(int position);

	unsigned int read(char* dest, unsigned int lenghtOfReadableContent);

	std::string read(unsigned int lenghtOfReadableContent);

	unsigned int readToEnd(char* dest);

	std::string readToEnd();

	std::string readAll();

	void write(std::string content);

	bool isOpen();

	void append(std::string content);

};





