#ifndef LOCATION_H
#define LOCATION_H
#include <iostream>
#include <string>
#include "webserv.h"

// Class definition 
class location
{
private: 
	std::string _index;
 
public: 
// Constructor 
	location(); 
	location( const location &rhs); 
	location(std::string newIndex);
	~location(); 
	location& operator=( const location &rhs); 
// Getters 
	std::string getIndex(); 
// Setters 
	void setIndex(std::string _index); 
// Output 
	void output(); 
}; 
 
#endif