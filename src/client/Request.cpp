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

bool	Request::appendBuffer(string recvBuffer) {
	cout << "recv buffer:" << recvBuffer << endl;

	_buffer.append(recvBuffer);
	if (this->getRequestHeader() == true) {
		this->parseBufferHeader();
		if (this->getRequestHeader() == false && this->getRequestBody() == false) {
			return false;
		}
		return true;
	}
	else if (this->getRequestHeader() == false && this->getRequestBody() == true) {
		cout << "check" << endl;
		stringstream ss(_buffer);
		this->parseBufferBody(&ss);
		this->output();
		cout << "check" << endl;
		if (this->getRequestBody() == false)
		{
			cout << "come here check" << endl;
			return false;
		}
		return true;
	}
	return false;
}

void	Request::parseBufferHeader() {
	stringstream	ss(_buffer);
	string			tmp;
	string			tmpBody;
	while (getline(ss, tmp)) {
		if (tmp.empty())
			continue ;
		else if (tmp.compare("\r") == 0) {
//			//cout << "this happens now" << endl;
			this->parseBufferBody(&ss);
			this->setupHeader();// buffer moet iets van tmp zijn
			return ;
			//			while (getline(ss, tmp)) {
//				this->setRequestBody(true);
////				_body.append(tmp);
//				if (tmp.find("\r") != string::npos)
//					_body.append("\n");
//				else if (tmp.compare("\r") == 0) {
//					this->setRequestBody(false);
//					break ;
//				}
//				_body.append(tmp);
//				_body.append("\n");
//			}
//			cout << "body :" << _body << endl;
//			return ;
		}
		else if (tmp.find("\r") == string::npos) {
//			//cout << "this is tmp:" << tmp ;
//			//cerr << ":inside\n";
			_buffer = tmp;
			return ;
		}
//		tmp = tmp.substr(0, tmp.size() - 1);
		_input.push_back(tmp);
	}
	_buffer.clear();
}

void	Request::setupHeader() {
//	stringstream ss(_body);
	string 	tmp;
	size_t 	contentLength;
	this->setAttributes();
	this->setHeaderContent();
	this->setRequestHeader(false);
	if (!getMethod().compare("POST")) {
		contentLength = (size_t)atol(this->getHeaderValue("Content-Length").c_str())
		setRequestBody(true);
	}
//	if (!getMethod().compare("POST")){
//		this->setRequestBody(true);
//		contentLength = (size_t)atol(this->getHeaderValue("Content-Length").c_str());
//		if (contentLength == _body.size())
//			this->setRequestBody(false);
//	}
	_buffer.clear();
}

void	Request::parseBufferBody(stringstream *ss) {
//	stringstream	ss(_buffer);
//	stringstream ss = *inputSs;
	string			tmp;

	while (getline(*ss, tmp)) {
		if (tmp.find("\r") == string::npos) {
			cout << "parseBUfferBody : " << _body << endl;
			_buffer = tmp;
			return ;
		}
		else if (tmp.compare("\r") == 0) {
			cout << "read everything in body : "<< _body << endl;
			this->setRequestBody(false);
			return ;
		}
//		else if ()
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
	cerr << getMethod() << endl;
	getline(ss, tmp, ' ');
	if (tmp.empty())
		throw WSException::BadRequest();
	setDir(tmp);
	getline(ss, tmp);
	if (tmp.empty())
		throw WSException::BadRequest();
	if (tmp.compare("HTTP/1.1\r")) {
		//cerr << " http version "  << tmp << endl;
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

    while (i < size) { // && _input[i].compare(0, 1,"\n")) {
//		//cout << "setHeaderContent:" << _input[i] << endl;
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
    //cout << "_method : " << _method << std::endl;
    //cout << "_dir : " << _dir << std::endl;
    //cout << "_protocol : " << _protocol << std::endl;
	//cout << "_requestHeader : " << _requestHeader << endl;
	//cout << "_requestBody : " << _requestBody << endl;
	//cout << "_body : " << _body << endl;
}
