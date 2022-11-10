#include "includes/config.hpp"
// Constructor initializes attributes to 0 by default 
config::config()
	: _address(0), _server_name(0), _root(0), _max_size(0), _location(0), _cgi(0)
{

}
 
config::config( const config& rhs)
{
	*this = rhs;
}

config::config(int newAddress, std::string newServer_name, std::string newRoot, unsigned long long newMax_size, location newLocation, std::string newCgi)
	: _address(newAddress), _root(newRoot), _max_size(newMax_size), _cgi(newCgi)
{
	_server_name.push_back(newServer_name);
}

config::~config()
{

}

config&	config::operator=( const config& rhs )
{
	return *this;
}

// Getters 
int config::getAddress() { return _address; }
//std::vector<std::string> config::getServer_name() { return _server_name; }
std::string config::getRoot() { return _root; }
unsigned long long config::getMax_size() { return _max_size; }
std::vector<location> config::getLocation() { return _location; }
std::string config::getCgi() { return _cgi; }
 
// Setters 
void config::setAddress(int newAddress) { _address = newAddress; }
void config::setServer_name(std::vector<std::string> newServer_name) { _server_name = newServer_name; }
void config::setRoot(std::string newRoot) { _root = newRoot; }
void config::setMax_size(unsigned long long newMax_size) { _max_size = newMax_size; }
void config::setLocation(std::vector<location> newLocation) { _location = newLocation; }
void config::setCgi(std::string newCgi) { _cgi = newCgi; }
 
// Output
void config::output()
{
  std::cout << "address : " << _address << std::endl; 
//  std::cout << "server_name : " << _server_name << std::endl;
  std::cout << "root : " << _root << std::endl; 
  std::cout << "max_size : " << _max_size << std::endl;
//  _location.output();
  std::cout << "cgi : " << _cgi << std::endl;
}


 
