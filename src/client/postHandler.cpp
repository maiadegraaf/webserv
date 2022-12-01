#include "Client.hpp"
#include <iostream>
#include <fstream>


void Client::createFileStorePost()
{

}
std::ofstream outfile ("test.txt");

outfile << "my text here!" << std::endl;

outfile.close();