#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <map>
#include <fstream>

using namespace std;

map<string, string> returnContentType( void );

ifstream::pos_type fileSize(const char* filename);

#endif