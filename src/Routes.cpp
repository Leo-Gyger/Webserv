//
// Created by Mano Segransan on 3/11/22.
//

#include "Routes.hpp"

Routes::Routes() : redir(false), path(false), cgi(false), methods(0) {}

Routes::~Routes() {}

void Routes::setMethods(int p) { this->methods = p; }

void Routes::setDefaultFile(const std::string &s) { this->default_file = s; }
std::string Routes::getDefaultFile() const { return this->default_file; }

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
