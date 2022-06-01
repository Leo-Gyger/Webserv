#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"
#include "Route.hpp"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <map>
#include <vector>

class Response
{
public:
	Response(const Response &);
	Response(const std::vector<Route> &, const Request &, const int &);

	~Response();

	Response &operator=(const Response &);

	Request &getResponse();

	void callCGI(const Request &, const int &);

	static std::map<std::string, std::string> buildCGIEnv(const Request &);

	static std::string Date();
private:
	Request response;
	std::string filename;
	Route r;

	std::string redirection();
	static std::string createStatusLine(int);
	void form_body(const std::string &);
	static std::string findType(std::string);
	void	put_method(const Request&, int&);
	void redirection(const std::string &, const std::string &);
	static std::string createFname(const std::string &, bool &);
	int findRoute(std::vector<Route> &route, int method);
	bool is_valid(const std::string &);
};
std::ostream &operator<<(std::ostream &, const Response &);

#endif
