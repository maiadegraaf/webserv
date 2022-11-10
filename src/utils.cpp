#include "includes/webserv.h"

void failure(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

int check_file_type(string filename)
{
	if (filename.substr(filename.find_last_of(".") + 1) == "conf")
		return (EXIT_SUCCESS);
	failure("Incorrect file type.  Only .conf files are accepted.");
	return (EXIT_FAILURE);
}

void	printStr(const string str)
{
	cout << str << endl;
}

string findNextWord(const string &line, size_t pos)
{
	size_t start = line.find_first_not_of(" \t", pos);
	size_t end = line.find_first_of(" \t", start);
	if (end == string::npos || start == string::npos)
	{
		cerr << line;
		failure(" does not contain enough information.");
	}
	return(line.substr(start, end - start));
}
