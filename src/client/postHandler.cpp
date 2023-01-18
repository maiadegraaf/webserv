#include "Client.hpp"
#include <iostream>
#include <fstream>

void Client::parsePostPlainRequest()
{
	string	req;
	int i = 0;
	stringstream ss(_request.getBody());

	while (getline(ss, req, '\n')) {
		req.erase(remove( req.begin(), req.end(), '\r' ),req.end());
		_postContent.push_back(vector<string>());
		_postContent[i].push_back(req.substr(0, req.find("=")));
		_postContent[i].push_back(req.substr(req.find("=") + 1, req.length()));
//		cerr << "postContent:: " << _postContent[i][1] << endl;
		i++;
	}
	while (i-- > 0)
		createFileStorePost(i);
}

void Client::decryptWwwForm(string &data)
{
	size_t pos;

	while ((pos = data.find("+")) != std::string::npos) {
		data.replace(pos, 1, " ");
	}
	while ((pos = data.find("%")) != std::string::npos) {
		if (pos <= data.length() - 3) {
			char replace[2] = {(char)(std::stoi("0x" + data.substr(pos+1,2), NULL, 16)), '\0'};
			data.replace(pos, 3, replace);
		}
	}
	while ((pos = data.find("&")) != std::string::npos) {
		data.replace(pos, 1, "\n");
	}
}

void Client::parsePostWwwRequest()
{
	string	req;
	int i = 0;
	string data = _request.getBody();
	decryptWwwForm(data);
	stringstream ss(data);

	while (getline(ss, req, '\n')) {
		_postContent.push_back(vector<string>());
		_postContent[i].push_back(req.substr(0, req.find("=")));
		_postContent[i].push_back(req.substr(req.find("=") + 1, req.length()));
		i++;
	}
	while (i-- > 0)
		createFileStorePost(i);
}

void Client::makeMapOfMultipartHeader(string tmp, int content_nb)
{
	string s;

	if (tmp.find(": ") < tmp.find("="))
	{
		tmp.erase(remove( tmp.begin(), tmp.end(), '\r' ),tmp.end());
		setHeaderMultipartValue(tmp.substr(0, tmp.find(":")), tmp.substr(tmp.find(":") + 2, tmp.length()),
								content_nb);
	}
	if (tmp.find("=") < tmp.find(": "))
	{
		s = tmp.substr(tmp.find("=") + 1, tmp.length());
		s.erase(remove( s.begin(), s.end(), '\"' ),s.end());
		s.erase(remove( s.begin(), s.end(), '\r' ),s.end());
		setHeaderMultipartValue(tmp.substr(0, tmp.find("=")), s, content_nb);
	}
}

void Client::parseHeaderMultipart(string *req, stringstream *ss, int content_nb, bool *endOfReq)
{
	string	tmp;
	size_t pos = 0;
	(void)_request;

	while (getline(*ss, *req, '\n'))
	{
//		cerr << "1:" << *req << endl;
		if (req->compare("--" + _request.getHeaderValue("boundary") + "--\r") == 0)
		{
			*endOfReq = true;
			break;
		}
		if (req->length() == 1)
			break;
		_headerMultipart.push_back( map<string, string>() );
		tmp = *req;
		while ((pos = req->find(";")) != string::npos) {
			tmp = req->substr(0, pos);
			makeMapOfMultipartHeader(tmp, content_nb);
			req->erase(0, pos + 2);
		}
		makeMapOfMultipartHeader(*req, content_nb);
	}
}


void Client::parsePostMultipartRequest()
{
	bool endOfReq = false;
	string data = _request.getBody();
	stringstream ss(data);
	int	content_nb = 0;
	string	req;
	string	contentFile;

//	cerr << data << endl;
	while (1) {
		parseHeaderMultipart(&req, &ss, content_nb, &endOfReq);
		if (endOfReq == true)
			break;
//		ofstream outfile(_headerMultipart[content_nb]["name"]);
		while (getline(ss, contentFile, '\n'))
		{
//			cerr << "2: " << contentFile << endl;
			if (contentFile.compare("--" + _request.getHeaderValue("boundary") + "\r") == 0) {
				break;
			}
			if (contentFile.compare("--" + _request.getHeaderValue("boundary") + "--\r") == 0)
			{
				endOfReq = true;
				break;
			}
//			outfile << contentFile;
//			outfile << endl;
		}
//		outfile.close();
		cout << content_nb << endl;
		cout << endOfReq << endl;
		content_nb++;
		sleep(1);
	}
}

void Client::createFileStorePost(int i)
{
	string uploadPath = getLocation(_request.getDir()).getUpload();
	uploadPath.append(_postContent[i][0]);

	ofstream outfile (uploadPath);
	outfile << _postContent[i][1];
	outfile.close();
}
