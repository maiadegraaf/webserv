#include "class.hpp"
// Constructor initializes attributes to 0 by default 
class::class()
	: _a(0)
{

}
 
class::class( const class& rhs)
{
	*this = rhs;
}
 
class::class(int newA) 
	: _a(newA)
{

}

class::~class()
{

}

class&	class::operator=( const class& rhs )
{
	return *this;
}

// Getters 
int class::getA() { return _a; }
 
// Setters 
void class::setA(int newA) { _a = newA; }
 
// Output
void class::output()
{
  std::cout << "a : " << _a << std::endl; 
}
 
