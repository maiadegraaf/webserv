#include "Request.hpp"

Request::Request()
        : _buffer(""), _method(""), _dir(""), _protocol(""), _body(""), _requestHeader(true), _requestBody(false) {}

Request::Request( const Request& rhs ) {
    *this = rhs;
}

Request::~Request() {}

Request&	Request::operator=( const Request& rhs )
{
	this->_buffer = rhs._buffer;
    this->_dir = rhs._dir;
    this->_method = rhs._method;
    this->_protocol = rhs._protocol;
    this->_input = rhs._input;
	this->_header = rhs._header;
	this->_body = rhs._body;
	this->_requestHeader = rhs._requestHeader;
	this->_requestBody = rhs._requestBody;

    return *this;
}

bool Request::checkIfBody() {
	string	tmp;
	int		line = _ss->tellg();

	getline(*_ss, tmp);
	if ((tmp.find("GET") == string::npos || tmp.find("POST") == string::npos || tmp.find("DELETE") == string::npos) && !tmp.empty()) {

		cout << "body fcnt" << tmp << endl;
		_ss->seekg(line , ios_base::beg);
		return true;
	}
	_ss->seekg(line , ios_base::beg);
	return false;
}

bool Request::checkIfHeader() {
	string	tmp;
	int		line = _ss->tellg();

	getline(*_ss, tmp);
	if (tmp.find("GET") != string::npos || tmp.find("POST") != string::npos ||tmp.find("DELETE") != string::npos)
	{
		cout << "Header fcnt" << tmp << endl;
		_ss->seekg(line , ios_base::beg);
		return true;
	}
	_ss->seekg(line , ios_base::beg);
	return false;
}

void	Request::parseBuffer() {
	string			tmp;

	while (getline(*_ss, tmp)) {
		if (tmp.empty())
			continue ;
		if (tmp.compare("\r") == 0) {
			this->setupHeader();
			if (checkIfHeader() == true) {
				break;
			}
			if (checkIfBody() == true) {
				this->parseBufferBody();
			}
		}
		_input.push_back(tmp);
	}
}

void	Request::setupHeader() {
	string 	tmp;

	this->setAttributes();
	this->setHeaderContent();
}

void	Request::parseBufferBody() {
	string			tmp;

	while (getline(*_ss, tmp)) {
		if (checkIfHeader() == true)
			break;
		_body.append(tmp);
		_body.append("\n");
	}
}

void	Request::setAttributes() {
	stringstream 	ss(_input[0]);
	string 			tmp;

	getline(ss, tmp, ' ');
	if (tmp.empty() || (tmp.compare("GET") && tmp.compare("POST") && \
	tmp.compare("DELETE"))) {
		throw WSException::MethodNotAllowed();
	}
	setMethod(tmp);
	getline(ss, tmp, ' ');
	if (tmp.empty())
		throw WSException::BadRequest();
	setDir(tmp);
	getline(ss, tmp);
	if (tmp.empty())
		throw WSException::BadRequest();
	if (tmp.compare("HTTP/1.1\r")) {
		cerr << " http version "  << tmp << endl;
		throw WSException::HTTPVersionNotAvailable();
	}
	setProtocol(tmp);
}

void	Request::setHeaderContent() {
	string	key;
	string	nextKey;
	string	value;
	string	nextValue;
    size_t	idx;
    size_t	i = 1;
    size_t	size = _input.size();

    while (i < size) {
		idx = _input[i].find(':', 0);
		if (idx == string::npos)
			break ;
		key = _input[i].substr(0, idx);
		value = _input[i].substr(idx + 2);
		value = value.substr(0, value.length() - 1);
		value = value.substr(0, value.find(";"));
		if (_input[i].find(";"))
		{
			nextKey = _input[i].substr(_input[i].find(';', 0) + 2, _input[i].find('=', 0) - _input[i].find(';', 0) - 2);
			nextValue = _input[i].substr(_input[i].find("=") + 1, _input[i].length() - 1);
			nextValue = nextValue.substr(0, nextValue.find(";"));
			nextValue.erase(remove( nextValue.begin(), nextValue.end(), '\r' ),nextValue.end());
			setHeaderValue(nextKey, nextValue);
		}
		value.erase(remove( value.begin(), value.end(), '\r' ),value.end());
		setHeaderValue(key, value);
		i++;
	}
}

// Output
void Request::output() {
    cout << "_method : " << _method << std::endl;
    cout << "_dir : " << _dir << std::endl;
    cout << "_protocol : " << _protocol << std::endl;
	cout << "_requestHeader : " << _requestHeader << endl;
	cout << "_requestBody : " << _requestBody << endl;
	cout << "_body : " << _body << endl;
}
