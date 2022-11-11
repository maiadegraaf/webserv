#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <map>

using namespace std;

map<string, string> ReturnContentType( void );

map<string, string> gContentType = ReturnContentType();
#endif