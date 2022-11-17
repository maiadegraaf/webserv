#include "Location.hpp"
// Constructor initializes attributes to 0 by default 
Location::Location()
	: _location("/"), _index("index.html")
{

}

Location::Location( const Location& rhs)
{
	*this = rhs;
}
 
Location::Location(const string& newLocation, const string& newIndex)
	: _location(newLocation), _index(newIndex)
{

}

Location::Location(const string& newLocation)
		: _location(newLocation), _index("index.html")
{

}

Location::~Location()
{

}

Location&	Location::operator=( const Location& rhs )
{
	(void)rhs;
	return *this;
}

// Getters 
string Location::getLocation() { return _location; }
string Location::getIndex() { return _index; }
 
// Setters 
void Location::setLocation(const string& newLocation) { _location = newLocation; }
void Location::setIndex(const string& newIndex) { _index = newIndex; }
 
// Output
void Location::output()
{
  std::cout << "location : " << _location << std::endl; 
  std::cout << "index : " << _index << std::endl; 
}
 
