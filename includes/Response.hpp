#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include <vector>
class Response
{
	private:
		std::string request;
		std::vector<unsigned char> body;
		int	size_body;
		bool	is_valid(std::string demande);
	public:
		Response(const Response& t);
		Response(void);
		std::string	answer(std::string& request);
		Response&	operator=(const Response& t);
		const std::vector<unsigned char>& get_body(void) const;
		int get_size(void) const;
		~Response(void);
};
std::ostream	&operator<<(std::ostream &ostream, const Response &d);

#endif

