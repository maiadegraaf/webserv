#ifndef CONFIG_H
#define CONFIG_H
#include "webserv.h"

class location;
class configParser;
// Class definition
class config
{
private:
	int _address;
	vector<string> _serverName;
	string _root;
	string _errorPage;
	unsigned long long _maxSize;
	vector<location> _location;
	string _cgi;

public:
// Constructor
	config();
	config( const config &rhs );
	config(const string& filename);
	~config();
	config& operator=( const config &rhs);
// Getters
	int getAddress();
//	vector<string> getServer_name();
	string getRoot();
	unsigned long long getMax_size();
    vector<location> getLocation();
	string getCgi();
// Setters
//	void setAddress(int _address);
//	void setServer_name(vector<string> _serverName);
//	void setRoot(string _root);
//	void setMaxSize(unsigned long long _maxSize);
//	void setLocation(vector<location> _location);
//	void setCgi(string _cgi);

	void setAddress(const string &line);
	void setServer_name(const string &line);
	void setRoot(const string &line);
	void setMaxSize(const string &line);
	void setLocation(const string &line);
	void setCgi(const string &line);
	void setErrorPage(const string &line);
// Output
	void output();

	void determineCase(const string &word, const string& line);

	typedef void (config::* MemFuncPtr)(const string &line);
};


#endif