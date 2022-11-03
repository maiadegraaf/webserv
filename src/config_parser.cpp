#include "includes/webserv.h"

int check_file_type(std::string filename)
{
	if (filename.substr(filename.find_last_of(".") + 1) == "conf")
		return (EXIT_SUCCESS);
	failure("Incorrect file type.  Only .conf files are accepted.");
	return (EXIT_FAILURE);
}

int	config_parser(std::string filename)
{
	check_file_type(filename);
	std::ifstream config_file;
	config_file.open(filename, std::ios::in);
	if (config_file.is_open())
	{
		std::string line;
		std::cout << filename <<  " has been opened!" << std::endl;
		while (getline(config_file, line)) {
			std::cout << line << std::endl;
		}
	}
	else
		failure("File could not be opened.");
	return (EXIT_SUCCESS);
}
