#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <ctime>

class Response
{
private:
	std::string request;
	std::vector<unsigned char> body;
	int size_body;
	std::string	findType(std::string demande);
	std::string	Date(void);
	bool is_valid(const std::string &demande);

public:
	Response(const Response &t);
	Response();
    const std::string&    getRequest(void) const;
	std::string answer(std::string &request);
	Response &operator=(const Response &t);
	const std::vector<unsigned char> &get_body() const;
	int get_size() const;
	std::string	get_bSize(void);
	~Response();
};
std::ostream &operator<<(std::ostream &ostream, const Response &d);

#endif
