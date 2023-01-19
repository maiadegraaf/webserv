#include "Location.hpp"
// Constructor initializes attributes to 0 by default
Location::Location()
	: _autoIndex(false)
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

Location::Location(const ConfigParser &confP)
		: _autoIndex(false)
{
	setDefaultMethod();
	for(size_t i = 0; i < confP.getServerContent().size(); i++)
	{
		string word = confP.findFirstWord(i);
        determineCase(word, confP, i);
	}
//	checkIfComplete();
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
    string methods[] = {
            "GET",
            "POST",
            "DELETE",
            "PUT",
            "PATCH"
    };
    cout << "index : " << _index << endl;
    if (_autoIndex)
        cout << "autoIndex : on" << endl;
    for (map<e_method, bool>::iterator i = _method.begin(); i != _method.end(); i++)
        cout << methods[i->first] << " : " << i->second << endl;
    cout << "upload : " << _upload << endl;
}

void Location::setIndex(const ConfigParser &confP, int line)
{
    string type = "index";
    size_t indLoc = confP.at(line).find(type);
    _index = findNextWord(confP.at(line), indLoc + type.length(), true);
}

void Location::setAutoIndex(const ConfigParser &confP, int line)
{
    string type = "autoindex";
    size_t indLoc = confP.at(line).find(type);
    string tmp = findNextWord(confP.at(line), indLoc + type.length(), true);
    if (tmp.compare("ON") == 0)
        _autoIndex = true;
    else if (tmp.compare("OFF") != 0)
        failure("Cannot recognize word after autoindex");
}

e_method Location::determineMethod(string s)
{
    if (s == "GET") return GET;
    if (s == "POST") return POST;
    if (s == "DELETE") return DELETE;
    if (s == "PUT") return PUT;
    if (s == "PATCH") return PATCH;
    return ERROR;
}

void Location::setMethod(const ConfigParser &confP, int line)
{
    string	type = "request_method";
    size_t	end = confP.at(line).find(type) + type.length();
    for(size_t i = 0; i < confP.at(line).length(); i++)
    {
        string s = findNextWord(confP.at(line), end, true);
        e_method methodType = determineMethod(s);
        if (methodType == ERROR)
        {
            cerr << s;
            failure(" is not an accepted method");
        }
        _method[determineMethod(s)] = true;
        end = confP.at(line).find(s) + s.length();
        i += end;
    }
}

void Location::setUpload(const ConfigParser &confP, int line)
{
    string type = "upload";
    size_t indLoc = confP.at(line).find(type);
    _upload = findNextWord(confP.at(line), indLoc + type.length(), true);
}

void Location::determineCase(const string& word, const ConfigParser &confP, int line)
{
	string words[] = {
            "index",
			"autoindex",
			"request_method",
			"upload"
	};
	LocMemFuncPtr setter[] = {
			&Location::setIndex,
			&Location::setAutoIndex,
			&Location::setMethod,
			&Location::setUpload
	};

	for(int i = 0; i < 4; i++)
	{
		if (word == words[i])
		{
			(this->*setter[i])(confP, line);
			break;
		}
	}
}

void Location::checkIfComplete(void) {
    if (_autoIndex && _index.empty())	{
        setIndex("index.html");
	}
}

