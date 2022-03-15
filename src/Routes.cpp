//
// Created by Mano Segransan on 3/11/22.
//

#include "Routes.hpp"
#include "cgi.hpp"
#include <unistd.h>


Routes::Routes() : redir(false), path(false), cgi(false), methods(0) {}

Routes::~Routes() {}

void Routes::setMethods(int p) { this->methods = p; }

void Routes::setDefault(const std::string& s) {this->def = s;}
const std::string Routes::getDefault(void) const {return this->def; }

int Routes::getMethods() const { return this->methods; }
void Routes::addMethods(int p) { this->methods |= p; }
void Routes::delMethods(int p) { this->methods &= ~p; }

void Routes::setUrl(const std::string &u) { this->url = u; }
std::string Routes::getUrl() const { return this->url; }

void Routes::setRoute(const std::string &r) { this->route = r; }
std::string Routes::getRoute() const { return this->route; }

void Routes::setRedir(bool r) { this->redir = r; }
bool Routes::getRedir() const { return this->redir; }

void Routes::setPath(bool p) { this->path = p; }
bool Routes::getPath() const { return this->path; }

void Routes::setCGI(bool c) { this->cgi = c; }
bool Routes::getCGI() const { return this->cgi; }


std::string Routes::callCGI(const std::string &body,
							const std::string &filepath) const
{
	int fd[2];
	std::string buffer;

	if (!this->cgi) return ("");
	if (pipe(fd) == -1) exit(EXIT_FAILURE);
	write(fd[1], body.c_str(), body.length());
	close(fd[1]);
	get_gci(buffer, filepath, fd);
	return (buffer);
}
