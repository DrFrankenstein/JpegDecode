#pragma once

#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>

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

template <typename T>
auto readIntoBuffer(std::istream& stream, std::streamsize length) -> std::unique_ptr<T>
{
	std::unique_ptr<std::byte> buffer { new std::byte[length] };
	if (!buffer)
		return buffer;

	stream.read(reinterpret_cast<char*>(buffer.get()), length);

	return buffer;
}

}

