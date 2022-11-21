#ifndef CONFIG_H
#define CONFIG_H
#include "webserv.h"
#include <map>

class Location;
class ConfigParser;
// Class definition
class Config
{
private:
	unsigned _address;
	vector<string> _serverName;
	string _root;
	unsigned long long _maxSize;
	map<string, string> _location;
	string _cgi;
	map<int, string> _errorPage;

public:
// Constructor
	Config(const Config &rhs );
    Config(vector<string> input);
    ~Config();
	Config& operator=(const Config &rhs);
// Getters
	unsigned int getAddress() const;
	const vector<string> &getServerName() const;
	const string &getRoot() const;
	unsigned long long int getMaxSize() const;
	const map<string, string> &getLocation() const;
	const string &getCgi() const;
	const map<int, string> &getErrorPage() const;
	string getLocation(string key)		{ return _location[key]; }
// Setters
	void setAddress(const vector<string>& input, int line);
	void setServer_name(const vector<string>& input, int line);
	void setRoot(const vector<string>& input, int line);
	void setMaxSize(const vector<string>& input, int line);
	void setLocation(const vector<string>& input, int line);
	void setCgi(const vector<string>& input, int line);
	void setErrorPage(const vector<string>& input, int line);
// Output
	void output();
//
	void determineCase(const string &word, const vector<string>& input, int line);
	typedef void (Config::* MemFuncPtr)(const vector<string>&, int);

	void checkIfComplete();

	void checkFileAccess(string filename);
};


#endif