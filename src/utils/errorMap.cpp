#include "../../inc/webserv.h"

map<int, string> setupErrorPages()
{
	map<int,string> errorPages;

	errorPages[400] = "error/400.html";
	errorPages[403] = "error/403.html";
	errorPages[404] = "error/404.html";
	errorPages[405] = "error/405.html";
	errorPages[418] = "error/418.html";
	return errorPages;
}
