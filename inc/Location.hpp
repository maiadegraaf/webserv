#ifndef LOCATION_H
#define LOCATION_H
#include "webserv.h"
 
// Class definition 
class Location
{
private: 
	string _location;
	string _index;
 
public: 
// Constructor 
	Location(); 
	Location( const Location &rhs); 
	Location(const string& newLocation, const string& newIndex);
	Location(const string& newLocation);
	~Location(); 
	Location& operator=( const Location &rhs); 
// Getters 
	string getLocation(); 
	string getIndex(); 
// Setters 
	void setLocation(const string& _location);
	void setIndex(const string& _index);
// Output 
	void output(); 
}; 
 
#endif
