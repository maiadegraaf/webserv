#include "ConfigParser.hpp"
// Constructor initializes attributes to 0 by default 
ConfigParser::ConfigParser()
	: _config_file(0), _server_content(0)
{

}
 
ConfigParser::ConfigParser(const ConfigParser& rhs)
{
	*this = rhs;
}

ConfigParser::ConfigParser(const string& filename)
{
	check_file_type(filename);
	_config_file.open(filename, ios::in);
	if (_config_file.is_open())
	{
		string line;
		while (getline(_config_file, line))
			_server_content.push_back(line);
	}
	else
		failure("File could not be opened.");
	_config_file.close();
}

ConfigParser::~ConfigParser() {}

ConfigParser&	ConfigParser::operator=(const ConfigParser& rhs )
{
    _server_content = rhs._server_content;
	return *this;
}

void	ConfigParser::output()
{
	for_each(_server_content.begin(), _server_content.end(), printStr);
}

int ConfigParser::findClosingBracket(size_t i, size_t pos)
{
    for(; i < _server_content.size(); i++)
    {
		if (_server_content[i].find('{', pos) != string::npos)
			i = findClosingBracket(i, _server_content[i].find('{', pos) + 1);
        else if(_server_content[i].find('}') != string::npos)
			return (int) i;
		pos = 0;
    }
    failure("Could not find closing bracket.");
    return (EXIT_FAILURE);
}

int ConfigParser::findClosingBracket(size_t i, size_t pos) const {
    for(; i < _server_content.size(); i++)
    {
        if (_server_content[i].find('{', pos) != string::npos)
            i = findClosingBracket(i, _server_content[i].find('{', pos) + 1);
        else if(_server_content[i].find('}') != string::npos)
            return (int) i;
        pos = 0;
    }
    failure("Could not find closing bracket.");
    return (EXIT_FAILURE);
}

string&	ConfigParser::operator[](int i)
{
	return _server_content[i];
}

string ConfigParser::findFirstWord(int i) const
{
	size_t start = _server_content[i].find_first_not_of(" \t");
	size_t end = _server_content[i].find_first_of(" \t", start);
    if (start == string::npos || end == string::npos)
        return "";
	return(_server_content[i].substr(start, end - start));
}

int	ConfigParser::findServer(int start, int *end)
{
	for(size_t i = start; i < getSize(); i++)
	{
		string s = findFirstWord((int) i);
		if (s == "server")
		{
			size_t brackLoc = s.find('{');
			if (brackLoc == s.length() - 1 || brackLoc == string::npos)
				*end = findClosingBracket(++i, 0);
			else
				*end = findClosingBracket(i, brackLoc + 1);
			return (int) i;
		}
	}
	return -1;
}

vector<string> ConfigParser::subVector(int first, int last)
{
    vector<string> subV;

    for(int i = first; i <= last; i++)
        subV.push_back(_server_content[i]);
    return subV;
}

vector<string> ConfigParser::subVector(int first, int last) const
{
    vector<string> subV;

    for(int i = first; i <= last; i++)
        subV.push_back(_server_content[i]);
    return subV;
}

string &ConfigParser::at(int key) const
{
    try {
        return const_cast<string &>(getServerContent()[key]);
    }
    catch (exception) {
        failure("key does not exist");
    }
    return const_cast<string &>(getServerContent()[key]);
}
