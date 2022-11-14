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
#include "Request.hpp"
#include "Response.hpp"
using namespace std;

#include "location.hpp"
#include "configParser.hpp"
#include "config.hpp"


//utils
void	failure(const char *message);
int 	check_file_type(const string& filename);
void	printStr(const string& str);
string findNextWord(const string &line, size_t pos);

#endif //WEBSERV_H
