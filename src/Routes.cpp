//
// Created by Mano Segransan on 3/11/22.
//

#include "Routes.hpp"

Routes::Routes() : methods(0) {}

Routes::~Routes() {}

void Routes::setMethods(int p) { this->methods = p; }
int Routes::getMethods() const { return this->methods; }
void Routes::addMethods(int p) { this->methods |= p; }
void Routes::delMethods(int p) { this->methods &= ~p; }

void Routes::setUrl(const std::string &u) { this->url = u; }
std::string Routes::getUrl() const { return this->url; }

void Routes::setPath(const std::string &p) { this->path = p; }
std::string Routes::getPath() const { return this->path; }
