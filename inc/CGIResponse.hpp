#ifndef CGIRESPONSE_H
#define CGIRESPONSE_H
#include <iostream>
#include <string>
#include "webserv.h"
 
// Class definition 
class CGIResponse : protected Response
{
private: 
	string _type;
 
public: 
// Constructor 
	CGIResponse(); 
	CGIResponse( const CGIResponse &rhs); 
	CGIResponse(string newType);
	~CGIResponse(); 
	CGIResponse& operator=( const CGIResponse &rhs); 
// Getters 
	string getType(); 
// Setters 
	void setType(string _type); 
// Output 
	void output(); 
}; 
 
#endif
