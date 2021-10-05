#pragma once

#include <exception>
#include <iostream>

namespace StreamUtils
{

template <typename T>
auto read(std::istream& stream) -> T
{
	T result;
	stream.read(reinterpret_cast<char*>(&result), sizeof(T));

	if (!stream)
		throw std::runtime_error { "premature end of file or read error" };

	return result;
}

}

