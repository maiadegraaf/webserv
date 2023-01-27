#include "../../inc/webserv.h"

map<int, string> setupErrorPages()
{
	map<int,string> errorPages;

	errorPages[400] = "../default/error/400.html";
	errorPages[403] = "../default/error/403.html";
	errorPages[404] = "../default/error/404.html";
	errorPages[405] = "../default/error/405.html";
	errorPages[413] = "../default/error/413.html";
	errorPages[418] = "../default/error/418.html";
	errorPages[500] = "../default/error/500.html";
	errorPages[501] = "../default/error/501.html";
	errorPages[505] = "../default/error/505.html";
	return errorPages;
}
