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

string addTabs(int tabs)
{
	string ret;
	for (int i = 0; i < tabs; i++)
		ret.append("    ");
	return ret;
}

int ConfigParser::findClosingBracket(size_t i, size_t pos)
{
    for(; i < _server_content.size(); i++)
    {
		if (_server_content[i].find('{', pos) != string::npos)
			i = findClosingBracket(i, _server_content[i].find('{', pos) + 1);
        else if(_server_content[i].find('}') != string::npos)
			return (i);
		pos = 0;
    }
    failure("Could not find closing bracket.");
    return (EXIT_FAILURE);
}

string&	ConfigParser::operator[](int i)
{
	return _server_content[i];
}

size_t ConfigParser::getSize() {
	return _server_content.size();
}

string ConfigParser::findFirstWord(int i)
{
	size_t start = _server_content[i].find_first_not_of(" \t");
	size_t end = _server_content[i].find_first_of(" \t", start);
	return(_server_content[i].substr(start, end - start));
}

int	ConfigParser::findServer()
{
	for(size_t i = 0; i < getSize(); i++)
	{
		string s = findFirstWord(i);
		if (s == "server")
		{
			size_t brackLoc = s.find('{');
			if (brackLoc == s.length() - 1 || brackLoc == string::npos)
				findClosingBracket(++i, 0);
			else
				findClosingBracket(i, brackLoc + 1);
			return i;
		}
	}
	failure("Could not locate server.");
	return EXIT_FAILURE;
}

vector<string> ConfigParser::subVector(int first, int last)
{
    vector<string> subV;

    for(int i = first; i <= last; i++)
        subV.push_back(_server_content[i]);
    return subV;
}
