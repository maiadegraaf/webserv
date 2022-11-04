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

//configParser::configParser(std::ifstream newConfig_file, std::vector<std::string> newServer_content)
//	: _config_file(newConfig_file), _server_content(newServer_content)
//{
//
//}

configParser::~configParser()
{

}

configParser&	configParser::operator=( const configParser& rhs )
{
	return *this;
}

void	configParser::output(void)
{
	std::for_each(_server_content.begin(), _server_content.end(), printStr);
}

int	configParser::openFile(std::string filename)
{
    check_file_type(filename);
    _config_file.open(filename, std::ios::in);
    if (_config_file.is_open())
    {
        std::string line;
        std::cout << filename <<  " has been opened!" << std::endl;
        while (getline(_config_file, line))
            _server_content.push_back(line);
    }
    else
        failure("File could not be opened.");
    _config_file.close();
	output();
    return (EXIT_SUCCESS);
}
