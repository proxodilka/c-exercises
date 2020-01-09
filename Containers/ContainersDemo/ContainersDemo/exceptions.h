#pragma once

class not_implemented_exception : public std::exception
{
public:
	virtual char const* what() const { return "Function not yet implemented."; }
};