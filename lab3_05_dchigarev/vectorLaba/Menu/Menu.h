#pragma once

#include <string>
#include <stdlib.h>
#include <iostream>
#include "../Vector/Vector.h"
#include <stdlib.h>
#include <functional>

typedef std::pair<std::string, std::function<void()>> menuItem;

class Menu {

public:
	Menu();

	Menu(const Vector<menuItem>& opt, std::string _header);

	void addOption(menuItem opt);

	void set_menu(const Vector<menuItem>& opt, std::string _header);

	void run();

	void set_header(std::string _header);

	void print_header();

private:
	Vector<menuItem> options;
	std::string state = "setup";
	std::string header = "Pick option:";
	std::istream& input = std::cin;
	std::ostream& output = std::cout;

	void print_options(bool is_clear_screen = true);

	void read_option();

	void print_error(std::exception e);
};

