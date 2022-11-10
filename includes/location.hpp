#ifndef LOCATION_H
#define LOCATION_H
#include "webserv.h"

// Class definition 
class location
{
private:
	string _location;
	string _index;
 
public: 
// Constructor 
	location(); 
	location( const location &rhs); 
	location(string newIndex);
	~location(); 
	location& operator=( const location &rhs); 
// Getters 
	string getIndex();
// Setters 
	void setIndex(string _index);
// Output 
	void output(); 
}; 
 
#endif