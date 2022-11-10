#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H
#include "webserv.h"

// Class definition
class configParser
{
private: 
	ifstream _config_file;
	vector<string> _server_content;
 
public: 
// Constructor 
	configParser(); 
	configParser( const configParser &rhs);
	configParser(const string& filename);
	~configParser(); 
	configParser& operator=( const configParser &rhs);

	void	output(void);

	int findClosingBracket(int i, size_t pos, int tab);

	string&	operator[](int i);

	size_t getSize();

	string findFirstWord(int i);

	int findServer();
};

#endif
