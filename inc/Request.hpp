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
	// (Con/De)structors
	public:
		Request();
		Request( const Request &rhs );
		~Request();
		Request& operator=( const Request &rhs );
		Request(string input);

	// Attributes
	private:
		string				_method;
		string				_dir;
		string				_protocol;
		vector<string>		_input;
		map<string, string> _content;
		string 				_file;

    // Getters
	public:
		string			getMethod()							{ return _method; }
		string			getDir() 							{ return _dir; }
		string			getProtocol()						{ return _protocol; }
		string			getContentValue(string key)			{ return _content[key]; }
		vector<string>	getInput()							{ return _input; }
		string 			getFile()							{ return _file; }

	// Setters
	public:
		void	setMethod(string new_method)				{ _method = new_method; }
		void	setDir(string new_dir)						{ _dir = new_dir; }
		void	setProtocol(string new_protocol)			{ _protocol = new_protocol; }
		void	setContentValue(string key, string value)	{ _content[key] = value; }

		void	setInput(string input);
		void	setAttributes();
		void	setContent();

    // Output
        void	output();
};
 
#endif
