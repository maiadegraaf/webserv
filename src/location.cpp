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
 
location::location(string newIndex)
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
string location::getIndex() { return _index; }
 
// Setters 
void location::setIndex(string newIndex) { _index = newIndex; }
 
// Output
void location::output()
{
  cout << "index : " << _index << endl;
}
 
