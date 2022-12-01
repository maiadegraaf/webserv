#include "Server.hpp"
#include "WebServ.hpp"
#include "webserv.h"

vector<Config> extractServer(const string& filename)
{
    ConfigParser    confParser(filename);
    vector<Config>  conf;
    int             end;
	int 			start = confParser.findServer(0, &end);
	while(start > -1)
    {
        Config  tmp(confParser.subVector(start, end));
        conf.push_back(tmp);
		start = confParser.findServer(end, &end);
	}
    if (conf.empty())
        failure("Could not identify server content");
    return (conf);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		failure("Incorrect INPUT");
	vector<Config> conf = extractServer(argv[1]);
	WebServ	webby(conf, returnContentType());
}
