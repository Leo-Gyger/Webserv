#include "header.hpp"

Header::Header(const int& s) : status(s)
{
	return;
}

Header::Header(const Header& t) : status(t.status), header(t.header)
{
	std::cout << "copy constructor" << std::endl;
	return;
}

Header& Header::operator=(const Header& t)
{
	std::cout << "copy assignment" << std::endl;
	this->status = t.status;
	this->header = t.header;
	return (*this);
}

Header::~Header(void)
{
	std::cout << "destructed" << std::endl;
	return;
}

