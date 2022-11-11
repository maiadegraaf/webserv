#include "inc/Location.hpp"
// Constructor initializes attributes to 0 by default 
Location::Location()
	: _location(0), _index(0)
{

}
 
Location::Location( const Location& rhs)
{
	*this = rhs;
}
 
Location::Location(string newLocation, string newIndex) 
	: _location(newLocation), _index(newIndex)
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
string Location::getLocation() { return _location; }
string Location::getIndex() { return _index; }
 
// Setters 
void Location::setLocation(string newLocation) { _location = newLocation; }
void Location::setIndex(string newIndex) { _index = newIndex; }
 
// Output
void Location::output()
{
  std::cout << "location : " << _location << std::endl; 
  std::cout << "index : " << _index << std::endl; 
}
 
