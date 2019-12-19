#include <iostream>
#include "ComplexReader.h"
#include "Complex.h"
#include "Menu.h"
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
	
	cout << "Введите путь к файлу:\n>";
	cin >> filename;

	fileHandler->open(filename);

	subMenu.run();
}

void readFromFile(File* fileHandler, bool isShowDialog=false) {
	int symbAmount = fileHandler->length();

	if (isShowDialog) {
		cout << "Введите кол-во символов для считывания:\n>";
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

	cout << "Установленная позиция: " << fileHandler->getPosition() << " | Длинна файла: " << fileHandler->length() << "\n"
		 << "Введите новую позицию:\n>";

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

	cout << "Сколько комплексных чисел записать?\n>";
	cin >> complexAmount;

	vector<Complex> data(complexAmount);
	generateComplex(data.data(), complexAmount);

	cout << "Сгенерированная последовательность:\n";

	for (Complex complex : data) {
		cout << "\t" << complex << "\n";
	}

	fileHandler->appendArray(data.data(), data.size());
	cout << "done.\n";

	system("pause");
}

void getComplexFileSize(ComplexReader* fileHandler) {
	cout << "Кол-во комплексных чисел в файле: " << fileHandler->size() << "\n";
	system("pause");
}


vector<menuItem> subOptionsFile = {
	{"Считать весь файл", []() {readFromFile(&file); } },
	{"Считать N символов", []() {readFromFile(&file, true); }},
	{"Записать данные", []() {readToFile(&file, false); }},
	{"Записать данные в конец", []() {readToFile(&file, true); }},
	{"Очистить файл", []() {clearFile(&file); }},
	{"Установить позицию", []() {setPosition(&file); }}
};

vector<menuItem> subOptionsComplexFile = {
	{"Считать объекты в вектор", []() {readComplex(&complexFile); } },
	{"Записать N объектов", []() {writeComplex(&complexFile); }},
	{"Очистить файл", []() {clearFile(&complexFile); }},
	{"Кол-во объектов", []() {getComplexFileSize(&complexFile); }}
};

vector<menuItem> mainOptions = {
	{"Выбрать файл", []() {
			subMenu.set_menu(subOptionsFile, "Выберите действие");
			pickFile(&file);
		}},
	{"Выбрать файл комплексного числа", []() {
			subMenu.set_menu(subOptionsComplexFile, "Выберите действие");
			pickFile(&complexFile);
		}}
};


void init() {
	setlocale(LC_ALL, "rus");

	mainMenu.set_menu(mainOptions, "Выберите действие:");
}

int main() {
	init();

	mainMenu.run();

}