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
using namespace std;

#include "ConfigParser.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "Utils.h"

//utils
void	failure(const char *message);
int 	check_file_type(const string& filename);
void	printStr(const string& str);
string	findNextWord(const string &line, size_t pos);
string  findFirstWord(int i, vector<string> v);
bool	fileAccess(string filename);

//utils/error map
map<int, string> setupErrorPages();

#endif //WEBSERV_H
