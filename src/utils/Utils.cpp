#include "Utils.h"

map<string, string>	returnContentType( void ) {
	map<string, string> CT;

	CT["aac"] = "audio/aac";
	CT["abw"] = "application/x-abiword";
	CT["arc"] = "application/x-freearc";
	CT["avif"] = "image/avif";
	CT["avi"] = "video/x-msvideo";
	CT["azw"] = "application/vnd.amazon.ebook";
	CT["bin"] = "application/octet-stream";
	CT["bmp"] = "image/bmp";
	CT["bz"] = "application/x-bzip";
	CT["bz2"] = "application/x-bzip2";
	CT["cda"] = "application/x-cdf";
	CT["csh"] = "application/x-csh";
	CT["css"] = "text/css";
	CT["csv"] = "text/csv";
	CT["doc"] = "application/msword";
	CT["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	CT["eot"] = "application/vnd.ms-fontobject";
	CT["epub"] = "application/epub+zip";
	CT["gz"] = "application/gzip";
	CT["gif"] = "image/gif";
	CT["html"] = "text/html";
	CT["ico"] = "image/vnd.microsoft.icon";
	CT["ics"] = "text/calender";
	CT["jar"] = "application/java-archive";
	CT["jpeg"] = "image/jpeg";
	CT["json"] = "application/json";
	CT["mp3"] = "audio/mpeg";
	CT["mp4"] = "video/mpeg";
	CT["mpkg"] = "application/vnd.apple.installer+xml";
	CT["png"] = "image/png";
	CT["pdf"] = "application/pdf";
	CT["sh"] = "application/x-sh";
	CT["tar"] = "application/x-tar";
	CT["txt"] = "text/plain";
	CT["xhtml"] = "application/xhtml+xml";
	CT["xlxs"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	CT["xls"] = "application/vnd.ms-excel";
	CT["xml"] = "application/xml";
	CT["zip"] = "application/zip";
	CT["7z"] = "application/x-7z-compressed";
	return CT;
}

ifstream::pos_type fileSize(const char* filename)
{
	ifstream in(filename, ifstream::ate | ifstream::binary);
	return in.tellg();
}
