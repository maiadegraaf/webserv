#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <map>
#include <fstream>

using namespace std;


typedef enum e_clientMode {
	request,
	response,
	disconnect
} 			clientMode;

map<string, string> returnContentType( void );

ifstream::pos_type fileSize(const char* filename);
//uintmax_t fileSize( const std::filesystem::path& p );

#endif