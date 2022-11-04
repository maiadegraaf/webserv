#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H
#include <iostream>
#include <string>
#include <vector>
#include "webserv.h"

// Class definition
class configParser
{
private: 
	std::ifstream _config_file;
	std::vector<std::string> _server_content;
 
public: 
// Constructor 
	configParser(); 
	configParser( const configParser &rhs); 
//	configParser(std::ifstream newConfig_file, std::vector<std::string> newServer_content);
	~configParser(); 
	configParser& operator=( const configParser &rhs);

    int	openFile(std::string filename);
	void	output(void);

};


 
#endif
