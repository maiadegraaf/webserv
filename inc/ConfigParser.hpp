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

	vector<string>	getServerContent() { return _server_content; };

	void	output(void);

	int findClosingBracket(size_t i, size_t pos);

	string&	operator[](int i);

	size_t getSize();

	string findFirstWord(int i);

    vector<string> subVector(int first, int last);

    int findServer(int start, int *end);
};

#endif
