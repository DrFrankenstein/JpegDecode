#pragma once

#include <iostream>

namespace JFIF
{
class JfifFile
{
	public:
	JfifFile(std::istream & stream);

	private:
	std::istream& _stream;
};
}