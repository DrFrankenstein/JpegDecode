#include <cstdlib>
#include <fstream>
#include <iostream>

#include "JFIF/JfifFile.hpp"

using JFIF::JfifFile;

int main(int argc, char* argv[])
{
	if (argc < 2)
		std::exit(1);

	std::ifstream stream { argv[1] };
	JfifFile file { stream };
}