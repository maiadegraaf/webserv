#include "CGIResponse.hpp"
// Constructor initializes attributes to 0 by default 
CGIResponse::CGIResponse()
	: _type(0)
{

}
 
CGIResponse::CGIResponse( const CGIResponse& rhs)
{
	*this = rhs;
}
 
CGIResponse::CGIResponse(string newType) 
	: _type(newType)
{

}

CGIResponse::~CGIResponse()
{

}

CGIResponse&	CGIResponse::operator=( const CGIResponse& rhs )
{
	return *this;
}

// Getters 
string CGIResponse::getType() { return _type; }
 
// Setters 
void CGIResponse::setType(string newType) { _type = newType; }
 
// Output
void CGIResponse::output()
{
  std::cout << "type : " << _type << std::endl; 
}
 
