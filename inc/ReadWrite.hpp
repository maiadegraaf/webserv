#ifndef READWRITE_H
#define READWRITE_H
#include <iostream>
#include <string>

enum e_RW
{
	READ,
	WRITE
};

// Class definition 
class ReadWrite
{
private: 
	e_RW _RW;
 
public: 
// Constructor 
	ReadWrite() : _RW(READ) {}
	ReadWrite( const ReadWrite &rhs) { *this = rhs; }
	ReadWrite(e_RW newRW) : _RW(newRW) {}
	~ReadWrite() {}
	ReadWrite& operator=( const ReadWrite &rhs );
// Getters 
	e_RW getRW() { return _RW; }
// Setters 
	void setRW(e_RW newRW) { _RW = newRW; }
	void setRead() { _RW = READ; }
	void setWrite() { _RW = WRITE; }
// Swap
	void swap();
// Output
	void output(); 
}; 
 
#endif
