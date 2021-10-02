module JFIF.Segment;

import std.core;

using std::istream, std::runtime_error, std::unique_ptr;

namespace JFIF
{
auto readSegment(istream& stream) -> unique_ptr<Segment>
{
	const auto ff = stream.get();
	if (ff != 0xFF)
	{
		throw runtime_error { "invalid segment marker (not 0xFF)" };
	}

	const auto type    = stream.get();
	const auto segment = new Segment;
	segment->type      = static_cast<SegmentType>(type);
	return unique_ptr<Segment>{ segment };
}

}