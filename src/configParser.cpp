#include "includes/configParser.hpp"
// Constructor initializes attributes to 0 by default 
configParser::configParser()
	: _config_file(0), _server_content(0)
{

}
 
configParser::configParser( const configParser& rhs)
{
	*this = rhs;
}

configParser::configParser(const string& filename)
{
	check_file_type(filename);
	_config_file.open(filename, ios::in);
	if (_config_file.is_open())
	{
		string line;
		cout << filename <<  " has been opened!" << endl;
		while (getline(_config_file, line))
			_server_content.push_back(line);
	}
	else
		failure("File could not be opened.");
	_config_file.close();
}

configParser::~configParser() {}

configParser&	configParser::operator=( const configParser& rhs )
{
	return *this;
}

void	configParser::output()
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

int configParser::findClosingBracket(int i, size_t pos, int tab)
{
    for(; i < _server_content.size(); i++)
    {
		if (_server_content[i].find('{', pos) != string::npos)
		{
			cout << addTabs(tab) << "{ at " << i << ", " << _server_content[i].find('{', pos) << std::endl;
			cout << addTabs(tab) << "search for a closing bracket initiated:" << endl;
			i = findClosingBracket(i, _server_content[i].find('{', pos) + 1, ++tab);
		}
        else if(_server_content[i].find('}') != string::npos)
		{
			cout << addTabs(tab) << "} at " << i << ", " << _server_content[i].find('}') << std::endl;
			return (i);
		}
		pos = 0;
    }
    failure("Could not find closing bracket.");
    return (EXIT_FAILURE);
}

string&	configParser::operator[](int i)
{
	return _server_content[i];
}

size_t configParser::getSize() {
	return _server_content.size();
}

string configParser::findFirstWord(int i)
{
	size_t start = _server_content[i].find_first_not_of(" \t");
	size_t end = _server_content[i].find_first_of(" \t", start);
	return(_server_content[i].substr(start, end - start));
}

int	configParser::findServer()
{
	for(int i = 0; i < getSize(); i++)
	{
		string s = findFirstWord(i);
		if (s == "server")
		{
			size_t brackLoc = s.find('{');
			if (brackLoc == s.length() - 1 || brackLoc == string::npos)
				findClosingBracket(++i, 0, 1);
			else
				findClosingBracket(i, brackLoc + 1, 1);
			return i;
		}
	}
	failure("Could not locate server.");
	return EXIT_FAILURE;
}
