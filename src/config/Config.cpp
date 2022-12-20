#include "Config.hpp"

Config::Config(const Config& rhs)
{
	*this = rhs;
}

Config::Config(const ConfigParser &confP)
	: _address(80), _maxSize(1), _errorPage(setupErrorPages())
{
	for(size_t i = 0; i < confP.getServerContent().size(); i++)
	{
		string word = confP.findFirstWord(i);
		determineCase(word, confP, i);
	}
    output();
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

const map<string, Location> &Config::getLocation() const {
	return _location;
}

const string &Config::getCgi() const {
	return _cgi;
}

const map<int, string> &Config::getErrorPage() const {
	return _errorPage;
}

// Setters 
void Config::setAddress(const ConfigParser &confP, int line)
{
	string	type = "listen";
	size_t	end = confP.at(line).find(type) + type.length();
	string	s = findNextWord(confP.at(line), end);
	int tmp = stoi(s);
	if (tmp < 0)
		failure("Listen must be a positive integer.");
	_address = tmp;
	if (!_address)
		failure("Listen is not correctly formatted.");
}

void Config::setServer_name(const ConfigParser &confP, int line)
{
	string	type = "server_name";
	size_t	end = confP.at(line).find(type) + type.length();
	for(size_t i = 0; i < confP.at(line).length(); i++)
	{
		string s = findNextWord(confP.at(line), end);
		_serverName.push_back(s);
		end = confP.at(line).find(s) + s.length();
		i += end;
	}
}

void Config::setRoot(const ConfigParser &confP, int line)
{
	string	type = "root";
	size_t	end = confP.at(line).find(type) + type.length();
	_root = findNextWord(confP.at(line), end);
}

void Config::setMaxSize(const ConfigParser &confP, int line)
{
	string	type = "client_max_body_size";
	size_t	end = confP.at(line).find(type) + type.length();
	_maxSize = stoull(findNextWord(confP.at(line), end));
	string	myLine = confP.at(line);
	for (size_t i = confP.at(line).find(type) + type.length(); i < myLine.size(); i++) {
		if (isalpha(myLine[i])) {
			if (myLine[i] == 'K')
				_maxSize *= 1000;
			else if (myLine[i] == 'M')
				_maxSize *= (1000 * 1000);
			else if (myLine[i] == 'G')
				_maxSize *= (1000 * 1000 * 1000);
			break ;
		}
	}
	if (!_maxSize)
		failure("client_max_body_size is not correctly formatted.");
}

void Config::setLocation(const ConfigParser &confP, int line)
{
    string	type = "location";
	size_t	end = confP.at(line).find(type) + type.length();
	string	loc = findNextWord(confP.at(line), end);
    size_t brackLoc = confP.at(line).find('{');
    int start = line;
    if (brackLoc == confP.at(line).length() - 1 || brackLoc == string::npos)
    {
        start++;
        end = confP.findClosingBracket(start, 0);
    }
    else
        end = confP.findClosingBracket(start, brackLoc + 1);
    ConfigParser tmp;
    tmp.setServerContent(confP.subVector(start, end - 1));
    Location    oneL(tmp);
    _location[loc] = oneL;
//	string	type = "location";
//	size_t	end = confP.at(line).find(type) + type.length();
//	string	loc = findNextWord(input[line], end);
//	string	ind;
//	for(int i = line + 1; input[i].find('}') == string::npos; i++)
//	{
//		type = "index";
//		size_t indLoc = input[i].find(type);
//		if (indLoc != string::npos)
//		{
//			ind = findNextWord(input[i], indLoc + type.length());
//			break;
//		}
//	}
//	if (!ind.empty() && !loc.empty())
//		_location[loc] = ind;
//	else if (!loc.empty() && ind.empty())
//		_location[loc] = "index.html";
//	else
//		failure("Index initialized incorrectly");
}

void Config::setCgi(const ConfigParser &confP, int line)
{
	string	type = "cgi";
	size_t end = confP.at(line).find(type) + type.length();
	_cgi = findNextWord(confP.at(line), end);
}

void Config::setErrorPage(const ConfigParser &confP, int line)
{
	string	type = "error_page";
	size_t	end = confP.at(line).find(type) + type.length();
	string tmp = findNextWord(confP.at(line), end);
	string page = findNextWord(confP.at(line), confP.at(line).find(tmp) + tmp.length());
	_errorPage[stoi(tmp)] = page;
}

// Output
void Config::output()
{
	//cout << "\n------------------------------------------------------\n" << endl;
	//cout << "address : " << _address << endl;
	//cout << "\nserver name(s) : " << endl;
	for_each(_serverName.begin(), _serverName.end(), printStr);
	//cout << "\nroot : " << _root << endl;
	//cout << "\nmax_size : " << _maxSize << endl;
    for (map<string, Location>::iterator i = _location.begin(); i != _location.end(); i++)
    {
        //cout << "\nLocation : " << i->first << endl;
        i->second.output();
    }
	//cout << "\ncgi : " << _cgi << endl;
	//cout << "\nError Page : "  << endl;
//	for (map<int, string>::iterator i = _errorPage.begin(); i != _errorPage.end(); i++)
//		cout << i->first << " : " << i->second << endl;
}

void Config::determineCase(const string& word, const ConfigParser &confP, int line)
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
	ConfMemFuncPtr setter[] = {
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
			(this->*setter[i])(confP, line);
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
//	for (Location::iterator i = _location.begin(); i != _location.end(); i++)
//	{
//		if (!fileAccess(_root + '/' + i->second))
//			failure(i->second.c_str());
//	}
    for (map<int, string>::iterator i = _errorPage.begin(); i != _errorPage.end(); i++)
	{
		if (!fileAccess(_root + '/' + i->second))
		{
			if (!fileAccess(i->second))
				failure(i->second.c_str());
		}
	}
}
