#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Routes.hpp"
#include <ctime>
#include <iostream>
#include <map>
#include <vector>

class Response
{
private:
	std::string request;
	std::vector<unsigned char> body;
	int size_body;
	std::string redirection(void);
	static std::string createStatusLine(int code);
	void form_body(const std::string &path);
	static std::string findType(std::string demande);
	static std::string Date();
	std::string redirection(const std::string &location);
	static std::string createFname(const std::string &header, bool &is_dir);
	std::string filename;
	bool findRoute(const std::vector<Routes> &r, const std::string &file_name);
	bool is_valid(std::string &demande);
	Routes r;

public:
	Response(const Response &t);
	const std::string &getRequest() const;
	Response(const std::vector<Routes> &r, const std::string &header);
	Response &operator=(const Response &t);
	const std::vector<unsigned char> &get_body() const;
	int get_size() const;
	static std::string callCGI(const std::string &header,
							   const std::string &requestBody,
							   const std::string &filepath);
	static std::map<std::string, std::string>
	buildCGIEnv(const std::string &header, const std::string &requestBody);
	~Response();
};
std::ostream &operator<<(std::ostream &ostream, const Response &d);

#endif
