#ifndef CGIRESPONSE_H
#define CGIRESPONSE_H
#include <iostream>
#include <string>
#include "Response.hpp"

class Response;
// Class definition
class CGIResponse : protected Response
{
private:
	string _type;

public:
// Constructor
	CGIResponse();
	CGIResponse( const CGIResponse &rhs);
	CGIResponse(const string& newType);
	CGIResponse(const string& newType, const string& filePath, const string& message, const string& contentType, int newSockFD, off_t fileSize);
	~CGIResponse();
	CGIResponse& operator=( const CGIResponse &rhs); 
// Getters 
	string getType(); 
// Setters 
	void setType(string _type); 
// Output 
	void output();

	bool exec();

	bool sendResponse();
};
 
#endif
