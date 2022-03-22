#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"
#include "Route.hpp"
#include <ctime>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

class Response
{
public:
	Response(const Response &t);
	Response(const std::vector<Route> &r, const Request &req);

	~Response();

	Response &operator=(const Response &t);

	const std::string &getRequest() const;

	const std::vector<unsigned char> &get_body() const;

	int get_size() const;

	std::string callCGI(const Request &req);

	static std::map<std::string, std::string> buildCGIEnv(const Request &req);

private:
	std::string request;
	std::vector<unsigned char> body;
	int size_body;
	std::string filename;
	Route r;

	std::string redirection(void);
	static std::string createStatusLine(int code);
	void form_body(const std::string &path);
	static std::string findType(std::string demande);
	static std::string Date();
	std::string redirection(const std::string &location);
	static std::string createFname(const std::string &header, bool &is_dir);
	bool findRoute(const std::vector<Route> &r, const std::string &file_name, const Request &req);
	bool is_valid(std::string &demande);
};
std::ostream &operator<<(std::ostream &ostream, const Response &d);

#endif
