#include "Request.hpp"

Request::Request()
        : _method(""), _dir(""), _protocol(""), _file("") {}

Request::Request( const Request& rhs ) {
    *this = rhs;
}

Request::~Request() {}

Request&	Request::operator=( const Request& rhs )
{
    this->_dir = rhs._dir;
    this->_method = rhs._method;
    this->_protocol = rhs._protocol;
    this->_content = rhs._content;
    this->_input = rhs._input;
    this->_file = rhs._file;
    return *this;
}

Request::Request(string input) {
    setInput(input);
    setAttributes();
    setContent();
}

// Setters
void Request::setInput(string input) {
    string vecPush;
    stringstream ss(input);

	cerr << input << endl;
	while (getline(ss, vecPush, '\n')) {
        _input.push_back(vecPush);
    }
}


void Request::setAttributes() {
    string			iString = _input[0];
    stringstream 	ss(iString);
    string 			tmp;

    getline(ss, tmp, ' ');
    if (!tmp.compare(0, 1, "") && \
	!tmp.compare(0, 4, "GET") && tmp.compare(0, 5, "POST") \
	&& tmp.compare(0, 7, "DELETE")) {
        throw WSException::MethodNotAllowed();
    }
    setMethod(tmp);
    getline(ss, tmp, ' ');
    if (!tmp.compare(0, 1, ""))
        throw WSException::BadRequest();
    setDir(tmp);
    getline(ss, tmp);
    if (!tmp.compare(0, 1, ""))
        throw WSException::BadRequest();
	if (!tmp.compare("HTTP/1.1")) {
		cerr << " http version "  << tmp << endl;
		throw WSException::HTTPVersionNotAvailable();
	}
    setProtocol(tmp);
}

void Request::setContent() {
	string	key;
	string	nextKey;
	string	value;
	string	nextValue;
    size_t	idx;
    size_t	i = 1;
    size_t	size = _input.size();

    while (i < size) { // && _input[i].compare(0, 1,"\n")) {
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
			setContentValue(nextKey, nextValue);
		}
		value.erase(remove( value.begin(), value.end(), '\r' ),value.end());
		setContentValue(key, value);
        i++;
    }
    _file.clear();
    for (size_t j = i + 1; j < size; j++) {
        _file.append(_input[j]);
        _file.append("\n");
    }
}

// Output
void Request::output() {
    std::cout << "_method : " << _method << std::endl;
    std::cout << "_dir : " << _dir << std::endl;
    std::cout << "_protocol : " << _protocol << std::endl;
}
