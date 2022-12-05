#ifndef LOCATION_H
#define LOCATION_H
#include <iostream>
#include <string>
#include <map>
#include "webserv.h"

class ConfigParser;
enum e_method
{
	GET,
	POST,
	DELETE,
	PUT,
	PATCH,
    ERROR
};

using namespace std;
// Class definition 
class Location
{
private:
    string _index;
	bool _autoIndex;
	map<e_method, bool> _method;
	string _upload;

public:
// Constructor
	Location();
	Location( const Location &rhs);
	Location(const string &newIndex, bool newAutoIndex, const string &newUpload);
	Location(const ConfigParser &confP);
	~Location();
	Location& operator=( const Location &rhs);
// Getters
	string getIndex() { return _index; }
	bool getAutoIndex() const { return _autoIndex; }
	map<e_method, bool> getMethod() { return _method; }
	string getUpload() { return _upload; }
// Setters
	void setIndex(const string &newIndex) { _index = newIndex; }
	void setAutoIndex(bool newAutoIndex) { _autoIndex = newAutoIndex; }
	void setMethod(const map<e_method, bool> &newMethod) { _method = newMethod; }
	void setUpload(const string &newUpload) { _upload = newUpload; };

    void setIndex(const ConfigParser &confP, int line);
	void setAutoIndex(const ConfigParser &confP, int line);
	void setMethod(const ConfigParser &confP, int line);
	void setUpload(const ConfigParser &confP, int line);

	void setDefaultMethod(void);

// Method Set
	void setGet()		{_method[GET] = true;}
	void setPost() 		{_method[POST] = true;}
	void setDelete()	{_method[DELETE] = true;}
	void setPut()		{_method[PUT] = true;}
	void setPatch()		{_method[PATCH] = true;}
// Output 
	void output();

    e_method determineMethod(string s);
    void determineCase(const string& word, const ConfigParser &confP, int line);

	// Typedef Member function
	typedef void (Location::* LocMemFuncPtr)(const ConfigParser&, int);
};
 
#endif