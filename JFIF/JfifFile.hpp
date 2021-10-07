#pragma once

#include "Segment.hpp"

#include <iostream>
#include <memory>
#include <vector>

namespace JFIF
{
class JfifFile
{
	public:
	JfifFile(std::istream & stream);

	private:
	using SegmentList = std::vector<std::unique_ptr<Segment>>;

	std::istream& _stream;
	SegmentList _segments;
};
}
