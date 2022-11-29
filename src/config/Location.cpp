#include "Location.hpp"
// Constructor initializes attributes to 0 by default
Location::Location()
	: _autoIndex(false),
{
	setDefaultMethod();
}
 
Location::Location( const Location& rhs)
{
	*this = rhs;
}

Location::Location(const string &newIndex, bool newAutoIndex, const string &newUpload)
	: _index(newIndex), _autoIndex(newAutoIndex), _upload(newUpload)
{
	setDefaultMethod();
}

Location::Location(const vector<string> &input)
		: _autoIndex(false)
{
	setDefaultMethod();
	for(size_t i = 0; i < input.size(); i++)
	{
		string word = findFirstWord(i, input);
		determineCase(word, input, i);
	}
	checkIfComplete();
}

Location::~Location()
{

}

Location&	Location::operator=( const Location& rhs )
{
	setIndex(rhs._index);
	setAutoIndex(rhs._autoIndex);
	setMethod(rhs._method);
	setUpload(rhs._upload);
	return *this;
}

// Setters
void Location::setDefaultMethod(void)
{
	_method[GET] = false;
	_method[POST] = false;
	_method[DELETE] = false;
	_method[PUT] = false;
	_method[PATCH] = false;
}

// Output
void Location::output()
{
  std::cout << "index : " << _index << std::endl; 
  std::cout << "autoIndex : " << _autoIndex << std::endl; 
  std::cout << "method : " << _method << std::endl; 
  std::cout << "upload : " << _upload << std::endl; 
}

void Location::setIndex(const vector<string> &input, int line)
{

}

void Location::setAutoIndex(const vector<string> &input, int line)
{

}

void Location::setMethod(const vector<string> &input, int line)
{

}

void Location::setUpload(const vector<string> &input, int line)
{

}

void Location::determineCase(const string& word, const vector<string>& input, int line)
{
	string words[] = {
			"index",
			"autoindex",
			"request_method",
			"upload",
	};
	LocMemFuncPtr setter[] = {
			&Location::setIndex,
			&Location::setAutoIndex,
			&Location::setMethod,
			&Location::setUpload,
	};

	for(int i = 0; i < 7; i++)
	{
		if (word == words[i])
		{
			(this->*setter[i])(input, line);
			break;
		}
	}
}