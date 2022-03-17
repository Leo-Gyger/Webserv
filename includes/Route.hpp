//
// Created by Mano Segransan on 3/11/22.
//

#ifndef WEBSERV_ROUTE_HPP
#define WEBSERV_ROUTE_HPP

#include <iostream>

#define GET 1   // 0b1
#define POST 2  // 0b10
#define DELETE 4// 0b100

class Route
{
public:
	Route();
	~Route();

	void setMethods(int p);
	int getMethods() const;

	void setDefaultFile(const std::string &s);
	std::string getDefaultFile() const;

	void addMethods(int p);
	void delMethods(int p);

	void setUrl(const std::string &u);
	std::string getUrl() const;

	void setRoute(const std::string &p);
	std::string getRoute() const;

	void setRedir(bool r);
	bool getRedir() const;

	void setPath(bool p);
	bool getPath() const;

	void setCGI(bool c);
	bool getCGI() const;

private:
	std::string url;
	std::string route;
	std::string default_file;
	bool redir;
	bool path;
	bool cgi;
	int methods;
};


#endif//WEBSERV_ROUTE_HPP
