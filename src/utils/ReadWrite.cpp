#include "ReadWrite.hpp"

ReadWrite&	ReadWrite::operator=( const ReadWrite& rhs )
{
	_RW = rhs.getRW();
	return *this;
}

// Output
void ReadWrite::output()
{
  std::cout << "RW : " << _RW << std::endl; 
}

void ReadWrite::swap()
{
	if (getRW() == WRITE)
	{
		_RW = READ;
		return ;
	}
	_RW = WRITE;
}
