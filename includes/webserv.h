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

#define BUFF_SIZE 300000
#define PORT 8080

void failure(const char *message);

#endif //WEBSERVE_WEBSERV_H
