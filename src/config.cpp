#include "includes/config.hpp"
// Constructor initializes attributes to 0 by default 
config::config()
	: _address(0), _serverName(0), _root(0), _maxSize(0), _location(0), _cgi(0)
{

}
 
config::config( const config& rhs)
{
	*this = rhs;
}

config::config(const string& filename)
{
	configParser conPar(filename);
	int i = conPar.findServer();
	for(; i < conPar.getSize(); i++)
	{
		string word = conPar.findFirstWord(i);
		determineCase(word, conPar[i]);
	}
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
//vector<string> config::getServer_name() { return _serverName; }
string config::getRoot() { return _root; }
unsigned long long config::getMax_size() { return _maxSize; }
vector<location> config::getLocation() { return _location; }
string config::getCgi() { return _cgi; }
 
// Setters 
void config::setAddress(const string &line)
{
	string	type = "listen";
	size_t end = line.find(type) + type.length();
	string s = findNextWord(line, end);
	_address = stoi(s);
	if (!_address)
		failure("Listen is not correctly formatted.");
}

void config::setServer_name(const string &line)
{
	string	type = "server_name";
	size_t end = line.find(type) + type.length();
	for(int i = 0; i < line.length(); i++)
	{
		string s = findNextWord(line, end);
		_serverName.push_back(s);
		end = line.find(s) + s.length();
	}
}

void config::setRoot(const string &line)
{
	string	type = "root";
	size_t end = line.find(type) + type.length();
	_root = findNextWord(line, end);
}

void config::setMaxSize(const string &line)
{
	string	type = "client_max_body_size";
	size_t end = line.find(type) + type.length();
	_maxSize = stoull(findNextWord(line, end));
	if (!_maxSize)
		failure("client_max_body_size is not correctly formatted.");
}

void config::setLocation(const string &line)
{
	cout << line << endl;
}

void config::setCgi(const string &line)
{
	string	type = "cgi";
	size_t end = line.find(type) + type.length();
	_cgi = findNextWord(line, end);

}

void config::setErrorPage(const string &line)
{
	cout << line << endl;
}

 
// Output
void config::output()
{
  cout << "address : " << _address << endl;
//  cout << "server_name : " << _serverName << endl;
  cout << "root : " << _root << endl;
  cout << "max_size : " << _maxSize << endl;
//  _location.output();
  cout << "cgi : " << _cgi << endl;
}

void config::determineCase(const string& word, const string& line)
{
	string words[] = {"listen", "server_name", "root", "location", "client_max_body_size", "error_page", "cgi"};
	MemFuncPtr setter[] = {&config::setAddress, &config::setServer_name, &config::setRoot, &config::setLocation, &config::setMaxSize, &config::setErrorPage, &config::setCgi};

	for(int i = 0; i < 7; i++)
	{
		if (word == words[i])
		{
			(this->*setter[i])(line);
			break;
		}
	}
}
