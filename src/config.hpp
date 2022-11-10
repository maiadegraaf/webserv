#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include <string>
#include <vector>
#include "location.hpp"
 
// Class definition 
class config
{
private: 
	int _address;
	std::vector<std::string> _server_name;
	std::string _root;
	unsigned long long _max_size;
	std::vector<location> _location;
	std::string _cgi;
 
public: 
// Constructor 
	config(); 
	config( const config &rhs); 
	config(int newAddress, std::string newServer_name, std::string newRoot, unsigned long long newMax_size, location newLocation, std::string newCgi);
	~config(); 
	config& operator=( const config &rhs); 
// Getters 
	int getAddress(); 
//	std::vector<std::string> getServer_name();
	std::string getRoot(); 
	unsigned long long getMax_size();
    std::vector<location> getLocation();
	std::string getCgi(); 
// Setters 
	void setAddress(int _address); 
	void setServer_name(std::vector<std::string> _server_name);
	void setRoot(std::string _root); 
	void setMax_size(unsigned long long _max_size); 
	void setLocation(std::vector<location> _location);
	void setCgi(std::string _cgi); 
// Output 
	void output();

};
 
#endif