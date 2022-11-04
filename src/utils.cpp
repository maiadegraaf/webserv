#include "includes/webserv.h"

void failure(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

int check_file_type(std::string filename)
{
	if (filename.substr(filename.find_last_of(".") + 1) == "conf")
		return (EXIT_SUCCESS);
	failure("Incorrect file type.  Only .conf files are accepted.");
	return (EXIT_FAILURE);
}

void	printStr(const std::string str)
{
	std::cout << str << std::endl;
}