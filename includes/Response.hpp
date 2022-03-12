#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <vector>

class Response
{
private:
	std::string request;
	std::vector<unsigned char> body;
	int size_body;
	bool is_valid(const std::string &demande);

public:
	Response(const Response &t);
	Response();
	std::string answer(std::string &request);
	Response &operator=(const Response &t);
	const std::vector<unsigned char> &get_body() const;
	int get_size() const;
	~Response();
};
std::ostream &operator<<(std::ostream &ostream, const Response &d);

#endif
