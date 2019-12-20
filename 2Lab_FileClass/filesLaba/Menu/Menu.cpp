#pragma once
#include "Menu.h"

using namespace std;

Menu::Menu() {}

Menu::Menu(const vector<menuItem>& opt, string _header) : options(opt), header(_header) {}

void Menu::addOption(menuItem opt) { 
	options.push_back(opt);
	if (state == "ready") {
		print_options();
	}
}

void Menu::set_menu(const vector<menuItem>& opt, string _header) {
	options = opt;
	header = _header;
}

void Menu::run() { state = "ready"; print_options(); };

void Menu::set_header(string _header) { header = _header; }

void Menu::print_options(bool is_clear_screen) {
	if (is_clear_screen) {
		system("cls");
	}

	output << header << "\n";
	for (int i = 0; i < options.size(); i++) {
		output << "\t" << i + 1 << ". " << options[i].first << "\n";
	}
	output << "\t" << options.size() + 1 << ". " << "Back\n";
	output << ">";
	read_option();
}

void Menu::read_option() {
	string choise;
	int choise_int;
	function<void()> callback;

	input.clear();
	input >> choise;
	try {
		choise_int = stoi(choise);

		if (choise_int == options.size() + 1) {
			return;
		}

		callback = options[choise_int - 1].second;
	}
	catch (exception e) {
		print_error(e);
		print_options(false);
		return;
	}

	system("cls");
	state = "executing";
	callback();
	state = "ready";
	print_options();
}

void Menu::print_error(exception e) {
	output << "error:\n" << e.what();
}



