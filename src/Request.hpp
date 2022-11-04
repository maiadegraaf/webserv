#ifndef REQUEST_H
#define REQUEST_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

class Request
{
	public:
	// (Con/De)structor
		Request();
		Request( const Request &rhs );
		~Request();
		Request& operator=( const Request &rhs );
		Request(string input);

	private:
		string				_method;
		string				_protocol;
		string				_dir;
		vector<string>		_input;
		map<string, string> _content;

	public:
    // Getters
		string	getMethod();
		string	getDir();
		string	getProtocol();
		string	getContentValue(string key);
    // Setters
		void	setInput(string input);
		void	setAttributes();
		void	setContent();
        void	setMethod(string _method);
        void	setDir(string _dir);
        void	setProtocol(string _protocol);
        void	setContentValue(string key, string value);
    // Output
        void	output();
};
 
#endif
