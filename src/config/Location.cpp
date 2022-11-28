#include "Location.hpp"
// Constructor initializes attributes to 0 by default 
Location::Location()
	: _index(0), _autoIndex(0), _upload(0)
{
	setDefaultMethod();
}
 
Location::Location( const Location& rhs)
{
	*this = rhs;
}
 
Location::Location(string newIndex, bool newAutoIndex, map<e_method, bool> newMethod, string newUpload) 
	: _index(newIndex), _autoIndex(newAutoIndex), _method(newMethod), _upload(newUpload)
{

}

Location::~Location()
{

}

Location&	Location::operator=( const Location& rhs )
{
	return *this;
}

// Getters 
string Location::getIndex() { return _index; }
bool Location::getAutoIndex() { return _autoIndex; }
map<e_method, bool> Location::getMethod() { return _method; }
string Location::getUpload() { return _upload; }
 
// Setters 
void Location::setIndex(string newIndex) { _index = newIndex; }
void Location::setAutoIndex(bool newAutoIndex) { _autoIndex = newAutoIndex; }
void Location::setMethod(map<e_method, bool> newMethod) { _method = newMethod; }

void Location::setDefaultMethod(void)
{
	_method[GET] = false;
	_method[POST] = false;
	_method[DELETE] = false;
	_method[PUT] = false;
	_method[PATCH] = false;
}

void Location::setUpload(string newUpload) { _upload = newUpload; }
 
// Output
void Location::output()
{
  std::cout << "index : " << _index << std::endl; 
  std::cout << "autoIndex : " << _autoIndex << std::endl; 
  std::cout << "method : " << _method << std::endl; 
  std::cout << "upload : " << _upload << std::endl; 
}
 
