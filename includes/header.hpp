#ifndef HEADER_HPP
# define HEADER_HPP

#include <iostream>

class Header
{
	private:
		int	status;
		std::string header;
	public:
		Header(const Header& t);
		Header(const int& s);
		Header(const int& s, const std::string&	body, const std::string&	request);
		Header&	operator=(const Header& t);
		~Header(void);
};
std::ostream	&operator<<(std::ostream &ostream, const Header &d);

#endif

