#ifndef CONFIG_H
#define CONFIG_H
#include "webserv.h"
#include "Location.hpp"
#include <map>

class Location;
class ConfigParser;
// Class definition
class Config
{
private:
	string _address;
	vector<string> _serverName;
	string _root;
	unsigned long long _maxSize;
	map<string, Location> _location;
	string _cgi;
	map<int, string> _errorPage;

public:
// Constructor
	Config(const Config &rhs );
    Config(const ConfigParser &confP);
    ~Config();
	Config& operator=(const Config &rhs);
// Getters
	string getAddress() const;
	const vector<string> &getServerName() const;
	const string &getRoot() const;
	unsigned long long int getMaxSize() const;
	const map<string, Location> &getLocation() const;
	const string &getCgi() const;
	const map<int, string> &getErrorPage() const;
//	string getLocation(string key)		{ return _location[key]; }
// Setters
	void setAddress(const ConfigParser &confP, int line);
	void setServer_name(const ConfigParser &confP, int line);
	void setRoot(const ConfigParser &confP, int line);
	void setMaxSize(const ConfigParser &confP, int line);
	void setLocation(const ConfigParser &confP, int line);
	void setCgi(const ConfigParser &confP, int line);
	void setErrorPage(const ConfigParser &confP, int line);
// Output
	void output();
//
	void determineCase(const string &word, const ConfigParser &confP, int line);

	void checkIfComplete();

//	void checkFileAccess(string filename);

	vector<string>		listDirectory(const string& dirRoot) const;
	static bool				indexChecker(vector<string> dirLs);
	void				createIndexFile(vector<string> dirLs, string dirRoot);

	// MemberFunction Typedef
	typedef void (Config::* ConfMemFuncPtr)(const ConfigParser&, int);
};


#endif