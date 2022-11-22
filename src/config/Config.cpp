#include "Config.hpp"

Config::Config(const Config& rhs)
{
	*this = rhs;
}

Config::Config(vector<string> input)
{
	_address = 80;
	_root = "";
	_maxSize = 1;
	_cgi = "";
	_errorPage = setupErrorPages();
	for(size_t i = 0; i < input.size(); i++)
	{
		string word = findFirstWord(i, input);
		determineCase(word, input, i);
	}
	checkIfComplete();
}

Config::~Config()
{

}

Config&	Config::operator=(const Config& rhs )
{
	_address = rhs._address;
	_serverName = rhs._serverName;
	_root = rhs._root;
	_maxSize = rhs._maxSize;
	_location = rhs._location;
	_cgi = rhs._cgi;
	_errorPage = rhs._errorPage;
	return *this;
}

// Getters 
unsigned Config::getAddress() const {
	return _address;
}

const vector<string> &Config::getServerName() const {
	return _serverName;
}

const string &Config::getRoot() const {
	return _root;
}

unsigned long long int Config::getMaxSize() const {
	return _maxSize;
}

const map<string, string> &Config::getLocation() const {
	return _location;
}

const string &Config::getCgi() const {
	return _cgi;
}

const map<int, string> &Config::getErrorPage() const {
	return _errorPage;
}

// Setters 
void Config::setAddress(const vector<string>& input, int line)
{
	string	type = "listen";
	size_t	end = input[line].find(type) + type.length();
	string	s = findNextWord(input[line], end);
	int tmp = stoi(s);
	if (tmp < 0)
		failure("Listen must be a positive integer.");
	_address = tmp;
	if (!_address)
		failure("Listen is not correctly formatted.");
}

void Config::setServer_name(const vector<string>& input, int line)
{
	string	type = "server_name";
	size_t	end = input[line].find(type) + type.length();
	for(size_t i = 0; i < input[line].length(); i++)
	{
		string s = findNextWord(input[line], end);
		_serverName.push_back(s);
		end = input[line].find(s) + s.length();
		i += end;
	}
}

void Config::setRoot(const vector<string>& input, int line)
{
	string	type = "root";
	size_t	end = input[line].find(type) + type.length();
	_root = findNextWord(input[line], end);
}

void Config::setMaxSize(const vector<string>& input, int line)
{
	string	type = "client_max_body_size";
	size_t	end = input[line].find(type) + type.length();
	_maxSize = stoull(findNextWord(input[line], end));
	if (!_maxSize)
		failure("client_max_body_size is not correctly formatted.");
}

void Config::setLocation(const vector<string>& input, int line)
{
	string	type = "location";
	size_t	end = input[line].find(type) + type.length();
	string	loc = findNextWord(input[line], end);
	string	ind;
	for(int i = line + 1; input[i].find('}') == string::npos; i++)
	{
		type = "index";
		size_t indLoc = input[i].find(type);
		if (indLoc != string::npos)
		{
			ind = findNextWord(input[i], indLoc + type.length());
			break;
		}
	}
	if (!ind.empty() && !loc.empty())
		_location[loc] = ind;
	else if (!loc.empty() && ind.empty())
		_location[loc] = "index.html";
	else
		failure("Index initialized incorrectly");
}

void Config::setCgi(const vector<string>& input, int line)
{
	string	type = "cgi";
	size_t end = input[line].find(type) + type.length();
	_cgi = findNextWord(input[line], end);
}

void Config::setErrorPage(const vector<string>& input, int line)
{
	string	type = "error_page";
	size_t	end = input[line].find(type) + type.length();
	string tmp = findNextWord(input[line], end);
	string page = findNextWord(input[line], input[line].find(tmp) + tmp.length());
	_errorPage[stoi(tmp)] = page;
}

// Output
void Config::output()
{
	cout << "\n------------------------------------------------------\n" << endl;
	cout << "address : " << _address << endl;
	cout << "\nserver name(s) : " << endl;
	for_each(_serverName.begin(), _serverName.end(), printStr);
	cout << "\nroot : " << _root << endl;
	cout << "\nmax_size : " << _maxSize << endl;
	cout << "\nlocation : " << endl;
	for (map<string, string>::iterator i = _location.begin(); i != _location.end(); i++)
		cout << i->first << " : " << i->second << endl;
	cout << "\ncgi : " << _cgi << endl;
	cout << "\nError Page : "  << endl;
	for (map<int, string>::iterator i = _errorPage.begin(); i != _errorPage.end(); i++)
		cout << i->first << " : " << i->second << endl;
}

void Config::determineCase(const string& word, const vector<string>& input, int line)
{
	string words[] = {
			"listen",
			"server_name",
			"root",
			"location",
			"client_max_body_size",
			"error_page",
			"cgi"
	};
	MemFuncPtr setter[] = {
			&Config::setAddress,
			&Config::setServer_name,
			&Config::setRoot,
			&Config::setLocation,
			&Config::setMaxSize,
			&Config::setErrorPage,
			&Config::setCgi
	};

	for(int i = 0; i < 7; i++)
	{
		if (word == words[i])
		{
			(this->*setter[i])(input, line);
			break;
		}
	}
}

void	Config::checkIfComplete()
{
	if (_root.empty())
		failure("Root is a required field.");
	if (_serverName.empty())
		_serverName.push_back("localhost");
	for (map<string, string>::iterator i = _location.begin(); i != _location.end(); i++)
	{
		if (!fileAccess(_root + '/' + i->second))
			failure(i->second.c_str());
	}
    for (map<int, string>::iterator i = _errorPage.begin(); i != _errorPage.end(); i++)
	{
		if (!fileAccess(_root + '/' + i->second))
		{
			if (!fileAccess(i->second))
				failure(i->second.c_str());
		}
	}
}
