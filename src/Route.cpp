//
// Created by Mano Segransan on 3/11/22.
//

#include "Route.hpp"

Route::Route()
	: default_file("/index.html"), client_max_body_size(1000000), redir(false),
	  path(false), cgi(false), methods(0), browse(false)
{}

Route::~Route() {}

void Route::setMethods(int p) { this->methods = p; }

void Route::setDefaultFile(const std::string &s) { this->default_file = s; }
std::string Route::getDefaultFile() const { return this->default_file; }

int Route::getMethods() const { return this->methods; }
void Route::addMethods(int p) { this->methods |= p; }
void Route::delMethods(int p) { this->methods &= ~p; }

void Route::setUrl(const std::string &u) { this->url = u; }
std::string Route::getUrl() const { return this->url; }

void Route::setRoute(const std::string &r) { this->route = r; }
std::string Route::getRoute() const { return this->route; }

void Route::setRedir(bool r) { this->redir = r; }
bool Route::getRedir() const { return this->redir; }

void Route::setPath(bool p) { this->path = p; }
bool Route::getPath() const { return this->path; }

void Route::setCGI(bool c) { this->cgi = c; }
bool Route::getCGI() const { return this->cgi; }

void Route::setMaxBodySize(const int &c) { this->client_max_body_size = c; }
int Route::getMaxBodySize() const { return this->client_max_body_size; }

Route::Route(const Route &r)
{
	this->url = r.url;
	this->route = r.route;
	this->default_file = r.default_file;
	this->client_max_body_size = r.client_max_body_size;
	this->redir = r.redir;
	this->path = r.path;
	this->cgi = r.cgi;
	this->methods = r.methods;
	this->browse = r.browse;
}

void Route::setBrowse(bool c) { this->browse = c; }
bool Route::getBrowse() const { return this->browse; }
