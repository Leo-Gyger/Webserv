#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include "Routes.hpp"
class Response
{
private:
	std::string request;
	std::vector<unsigned char> body;
	int size_body;
	std::string	redirection(void);
	static std::string	createStatusLine(int code);
	void	form_body(const std::string& path);
	static std::string	findType(std::string demande);
	static std::string	Date(void);
	bool is_valid(const std::string &demande);
	Routes r;

public:
	Response(const Response &t);
	Response(const std::vector<Routes>& r);
    const std::string&    getRequest(void) const;
	std::string answer(std::string &request);
	Response &operator=(const Response &t);
	const std::vector<unsigned char> &get_body() const;
	int get_size() const;
	std::string	get_bSize(void) const;
	~Response();
};
std::ostream &operator<<(std::ostream &ostream, const Response &d);

#endif
