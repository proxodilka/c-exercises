#include <iostream>
#include "File/File.h"
#include "ComplexReader/ComplexReader.h"
#include "Complex/Complex.h"
#include "Menu/Menu.h"
#include <algorithm>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;

Menu mainMenu, subMenu;
File file;
ComplexReader complexFile;


void generateComplex(Complex* destination, int n) {
	for (int i = 0; i < n; i++) {
		destination[i] = { (double)(rand() % 20), (double)(rand() % 20) };
	}
}

void pickFile(File* fileHandler) {
	string filename;
	
	cout << "Enter file path:\n>";
	cin >> filename;

	fileHandler->open(filename);

	subMenu.run();
}

void readFromFile(File* fileHandler, bool isShowDialog=false) {
	int symbAmount = fileHandler->length();

	if (isShowDialog) {
		cout << "How many symbols you want to read:\n>";
		cin >> symbAmount;
	}
	else {
		fileHandler->seek(0);
	}

	
	string fileContent = fileHandler->read(symbAmount);
	cout << fileContent;

	system("pause");
}


void setPosition(File* fileHandler) {
	int newPosition = 0;

	cout << "Current position: " << fileHandler->getPosition() << " | File length: " << fileHandler->length() << "\n"
		 << "Enter new position:\n>";

	cin >> newPosition;
	
	fileHandler->seek(newPosition);
}

void readToFile(File* fileHandler, bool isAppending) {
	string content = "", curString="";

	while (getline(cin, curString)) {
		content += curString + "\n";
	}

	if (isAppending) {
		fileHandler->append(content);
	}
	else {
		fileHandler->write(content);
	}
	
}

void clearFile(File* fileHandler) {
	fileHandler->clear();
}

void readComplex(ComplexReader* fileHandler) {

	vector<Complex> data(fileHandler->size());
	fileHandler->getAsArray(data.data());

	cout << "vector<Complex> data : \n";
	for (Complex complex : data) {
		cout << "\t" << complex << "\n";
	}

	system("pause");
}

void writeComplex(ComplexReader* fileHandler) {
	int complexAmount = 0;

	cout << "How many complex numbers to write?\n>";
	cin >> complexAmount;

	vector<Complex> data(complexAmount);
	generateComplex(data.data(), complexAmount);

	cout << "Generated sequence:\n";

	for (Complex complex : data) {
		cout << "\t" << complex << "\n";
	}

	fileHandler->appendArray(data.data(), data.size());
	cout << "done.\n";

	system("pause");
}

void getComplexFileSize(ComplexReader* fileHandler) {
	cout << "Amount of complex numbers at file: " << fileHandler->size() << "\n";
	system("pause");
}


vector<menuItem> subOptionsFile = {
	{"Read all file", []() {readFromFile(&file); } },
	{"Read N symbols", []() {readFromFile(&file, true); }},
	{"Write data", []() {readToFile(&file, false); }},
	{"Append data", []() {readToFile(&file, true); }},
	{"Clear file", []() {clearFile(&file); }},
	{"Set position", []() {setPosition(&file); }}
};

vector<menuItem> subOptionsComplexFile = {
	{"Get objects as vector", []() {readComplex(&complexFile); } },
	{"Write N objects", []() {writeComplex(&complexFile); }},
	{"Clear file", []() {clearFile(&complexFile); }},
	{"Amount of objects", []() {getComplexFileSize(&complexFile); }}
};

vector<menuItem> mainOptions = {
	{"Pick file", []() {
			subMenu.set_menu(subOptionsFile, "Pick option");
			pickFile(&file);
		}},
	{"Pick file with complex numbers", []() {
			subMenu.set_menu(subOptionsComplexFile, "Pick option");
			pickFile(&complexFile);
		}}
};


void init() {
	setlocale(LC_ALL, "rus");

	mainMenu.set_menu(mainOptions, "Pick option:");
}

int main() {
	init();

	mainMenu.run();

}