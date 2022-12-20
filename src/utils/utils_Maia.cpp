#include "webserv.h"

void failure(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

int check_file_type(const string& filename)
{
	if (filename.substr(filename.find_last_of('.') + 1) == "conf")
		return (EXIT_SUCCESS);
	failure("Incorrect file type.  Only .conf files are accepted.");
	return (EXIT_FAILURE);
}

void	printStr(const string& str)
{
	cout << str << endl;
}

string findNextWord(const string &line, size_t pos)
{
	size_t start = line.find_first_not_of(" \t", pos);
	size_t end = line.find_first_of(" \t", start);
	if (start == string::npos)
	{
		//cerr << line;
		failure(": does not contain enough information.");
	}
	if (end == string::npos)
		end = line.find_first_of(';', start);
	if (end == string::npos)
	{
		//cerr << line;
		failure(": could not identify terminating semi-colon");
	}
	return(line.substr(start, end - start));
}

bool fileAccess(string filename)
{
	ifstream file;
    filename.insert(0, DIRECTORY);
	file.open(filename);
	if (file.is_open())
	{
		file.close();
		return true;
	}
	return false;
}

string findFirstWord(int i, vector<string> v)
{
    size_t start = v[i].find_first_not_of(" \t");
    size_t end = v[i].find_first_of(" \t", start);
    if (start == string::npos || end == string::npos)
        return "";
    return(v[i].substr(start, end - start));
}
