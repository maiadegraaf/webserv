#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H
#include "webserv.h"

// Class definition
class ConfigParser
{
private: 
	ifstream _config_file;
	vector<string> _server_content;
 
public: 
// Constructor 
	ConfigParser();
	ConfigParser(const ConfigParser &rhs);
	ConfigParser(const string& filename);
	~ConfigParser();
	ConfigParser& operator=(const ConfigParser &rhs);

// Setters
//    void                    setConfigFile(const ifstream &configFile)               { _config_file(configFile); }
    void                    setServerContent(const vector<string> &serverContent)   { _server_content = serverContent; }

// Getters
    const vector<string>    &getServerContent() const                               { return _server_content; }
    const ifstream          &getConfigFile() const                                  { return _config_file; }
    size_t                  getSize()                                               { return _server_content.size(); }

// Get at
    string&	operator[](int i);
    string &at(int key) const;

// Output
    void	output(void);

// Closing Bracket
	int findClosingBracket(size_t i, size_t pos);
    int findClosingBracket(size_t i, size_t pos) const;

    string findFirstWord(int i) const;

    vector<string> subVector(int first, int last);
    vector<string> subVector(int first, int last) const;

    int findServer(int start, int *end);
};

#endif
