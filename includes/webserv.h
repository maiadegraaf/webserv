#ifndef WEBSERVE_WEBSERV_H
#define WEBSERVE_WEBSERV_H
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "configParser.hpp"

//utils
void failure(const char *message);
int check_file_type(std::string filename);
void	printStr(const std::string str);

#endif //WEBSERVE_WEBSERV_H
