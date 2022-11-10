#ifndef CLASS_H
#define CLASS_H
#include <iostream>
#include <string>
 
// Class definition 
class class
{
private: 
	int _a;
 
public: 
// Constructor 
	class(); 
	class( const class &rhs); 
	class(int newA);
	~class(); 
	class& operator=( const class &rhs); 
// Getters 
	int getA(); 
// Setters 
	void setA(int _a); 
// Output 
	void output(); 
}; 
 
#endif
