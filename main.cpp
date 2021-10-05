#include <cstdlib>
#include <fstream>
#include <iostream>

#include "JFIF/JfifFile.hpp"

using std::exit, std::ifstream;
using JFIF::JfifFile;

int main(int argc, char* argv[])
{
	if (argc < 2)
		exit(1);

	ifstream stream { argv[1] };
	JfifFile file { stream };
}