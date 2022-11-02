#include "includes/location.hpp"
// Constructor initializes attributes to 0 by default 
location::location()
	: _index(0)
{

}
 
location::location( const location& rhs)
{
	*this = rhs;
}
 
location::location(std::string newIndex) 
	: _index(newIndex)
{

}

location::~location()
{

}

location&	location::operator=( const location& rhs )
{
	return *this;
}

// Getters 
std::string location::getIndex() { return _index; }
 
// Setters 
void location::setIndex(std::string newIndex) { _index = newIndex; }
 
// Output
void location::output()
{
  std::cout << "index : " << _index << std::endl; 
}
 
