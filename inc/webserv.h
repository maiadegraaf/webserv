#ifndef WEBSERV_H
#define WEBSERV_H
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <fstream>
#include <vector>
#include <map>
#include <dirent.h>
#include <stdlib.h>


using namespace std;

#include "ConfigParser.hpp"
#include "Config.hpp"
#include "Utils.h"

//#define DIRECTORY "/Users/mgraaf/Documents/webserv/"

//utils
void	        failure(const char *message);
int 	        check_file_type(const string& filename);
void	        printStr(const string& str);
string	        findNextWord(const string &line, size_t pos, bool conf);
string          findFirstWord(int i, vector<string> v);
bool	        fileAccess(string filename);
bool	        directoryAccess(const string& directory);
string	        extension(string filename);
char            **vectorToArr(vector<string> v);
vector<string>  splitStr(const string& s, string del);
void	        setDeleteHTMLResponse(const string& filePath);
vector<string>  parse_envp(char** envp);


//utils/error map
map<int, string> setupErrorPages();

#endif //WEBSERV_H
