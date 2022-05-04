//
// Created by Mano Segransan on 3/11/22.
//

#ifndef WEBSERV_ROUTE_HPP
#define WEBSERV_ROUTE_HPP

#include <iostream>
#include <vector>
#define GET 1    // 0b1
#define POST 2   // 0b10
#define DELETE 4 // 0b100
#define PUT 8    // 0b1000
#define HEAD 16  // 0b10000

class Route
{
public:
	Route();
	Route(const Route &);
	~Route();

	void setMethods(int);
	int getMethods() const;

	void setDefaultFile(const std::string &);
	std::string getDefaultFile() const;

	void addMethods(int);
	void delMethods(int);

	void setUrl(const std::string &);
	std::string getUrl() const;

	void setRoute(const std::string &);
	std::string getRoute() const;

	void setMaxBodySize(const int &);
	int getMaxBodySize() const;

	void setRedir(bool r);
	bool getRedir() const;

	void setPath(bool p);
	bool getPath() const;

	void setCGI(bool c);
	bool getCGI() const;
	std::vector<std::string> method;

private:
	std::string url;
	std::string route;
	std::string default_file;
	int client_max_body_size;
	bool redir;
	bool path;
	bool cgi;
	int methods;
};


#endif//WEBSERV_ROUTE_HPP
