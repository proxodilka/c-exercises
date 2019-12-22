#include <iostream>
#include "Vector/Vector.h"
#include "Menu/Menu.h"
#include "Complex/Complex.h"

using namespace std;


Menu mainMenu, subMenu;
int pickedVector = 0;

Vector<Vector<Complex>> vectorStorage;

void generateComplex(Complex* destination, int n) {
	for (int i = 0; i < n; i++) {
		destination[i] = { (double)(rand() % 20), (double)(rand() % 20) };
	}
}

void updateMainMenuHeader() {

	string newHeader = "";

	for (int i = 0; i < vectorStorage.size(); i++) {
		newHeader += "v" + to_string(i) + " size: " + to_string(vectorStorage[i].size()) + " | ";
	}
	newHeader += "\n==================================================================\n";
	newHeader += "Pick option:\n";

	mainMenu.set_header(newHeader);
}

void createVector() {
	Vector<Complex> tmpVector;

	vectorStorage.push_back(tmpVector);
	updateMainMenuHeader();
}

void pickVector() {
	mainMenu.print_header();

	cin >> pickedVector;
	subMenu.run();
}

void removeVector() {
	vectorStorage.pop_back();
	updateMainMenuHeader();
}

Vector<menuItem> mainOptions = {
	{"Create vector", []() {createVector(); }},
	{"Pick vector", []() {pickVector(); }},
	{"Remove vector", []() {removeVector(); }}
};


void printPickedVector() {
	cout << "v" << pickedVector << ":\n";
	for (Complex complex : vectorStorage[pickedVector]) {
		cout << "\t" << complex << "\n";
	}

	system("pause");
}

void addToVector() {
	int complexAmount = rand() % 6 + 1;

	Vector<Complex> tmpComplex(complexAmount, {0,0});

	generateComplex(tmpComplex.begin(), tmpComplex.size());

	cout << "Generated sequence: \n";
	for (Complex complex : tmpComplex) {
		cout << "\t" << complex << "\n";
		vectorStorage[pickedVector].push_back(complex);
	}

	cout << "Added!\n";
	updateMainMenuHeader();
	system("pause");
}

void removeFromVector() {
	printPickedVector();

	int indexToRemove = 0;

	cout << "\nWhich element you want to delete?\n>";
	cin >> indexToRemove;

	vectorStorage[pickedVector].removeAt(indexToRemove);
	updateMainMenuHeader();
}

Vector<menuItem> subOptions = {
	{"Show vector", []() {printPickedVector(); }},
	{"Push N complex numbers to vector", []() {addToVector(); }},
	{"Remove element from vector", []() {removeFromVector(); }},
};


void init() {
	setlocale(LC_ALL, "rus");

	mainMenu.set_menu(mainOptions, "Pick option:");
	subMenu.set_menu(subOptions, "Pick option:");
}


int main() {
	init();

	mainMenu.run();	

}
