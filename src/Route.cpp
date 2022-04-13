//
// Created by Mano Segransan on 3/11/22.
//

#include "Route.hpp"

Route::Route() : default_file("index.html"), redir(false), path(false), cgi(false), methods(0) {}

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
