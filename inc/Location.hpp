#ifndef LOCATION_H
#define LOCATION_H
#include <iostream>
#include <string>
#include <map>
#include "webserv.h"

enum e_method
{
	GET,
	POST,
	DELETE,
	PUT,
	PATCH
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
	Location(const vector<string> &input);
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

	void setIndex(const vector<string>& input, int line);
	void setAutoIndex(const vector<string>& input, int line);
	void setMethod(const vector<string>& input, int line);
	void setUpload(const vector<string>& input, int line);

	void setDefaultMethod(void);

// Method Set
	void setGet()		{_method[GET] = true;}
	void setPost() 		{_method[POST] = true;}
	void setDelete()	{_method[DELETE] = true;}
	void setPut()		{_method[PUT] = true;}
	void setPatch()		{_method[PATCH] = true;}
// Output 
	void output();

	void determineCase(const string& word, const vector<string>& input, int line);

	// Typedef Member function
	typedef void (Location::* LocMemFuncPtr)(const vector<string>&, int);
};
 
#endif
