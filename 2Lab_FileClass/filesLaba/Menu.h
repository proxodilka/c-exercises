#pragma once

#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <functional>

using namespace std;

typedef pair<string, function<void()>> menuItem;

class Menu {

public:
	Menu() {};
	Menu(const vector<menuItem>& opt, string _header) : options(opt), header(_header) {};
	void addOption(menuItem opt) { options.push_back(opt); if (state == "ready") print_options(); };

	void set_menu(const vector<menuItem>& opt, string _header) {
		options = opt;
		header = _header;
	}

	void run() { state = "ready"; print_options(); };
	void set_header(string _header) { header = _header; }

private:
	vector<menuItem> options;
	string state = "setup";
	string header = "Выберите действие:";
	istream& input = std::cin;
	ostream& output = std::cout;

	void print_options(bool is_clear_screen = true) {
		if (is_clear_screen) {
			system("cls");
		}

		output << header << "\n";
		for (int i = 0; i < options.size(); i++) {
			output << "\t" << i + 1 << ". " << options[i].first << "\n";
		}
		output << "\t" << options.size() + 1 << ". " << "Выход\n";
		output << ">";
		read_option();
	}

	void read_option() {
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
			print_error();
			print_options(false);
			return;
		}

		system("cls");
		state = "executing";
		callback();
		state = "ready";
		print_options();
	}

	void print_error() {
		output << "error\n";
	}
};