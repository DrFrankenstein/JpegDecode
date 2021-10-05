#include "Segment.hpp"

#include "../StreamUtils/readers.hpp"

#include <algorithm>
#include <array>
#include <boost/endian/arithmetic.hpp>
#include <boost/endian/conversion.hpp>
#include <exception>
#include <iostream>
#include <memory>

using boost::endian::big_to_native, boost::endian::big_uint8_t, boost::endian::big_uint16_t;
using std::array, std::istream, std::runtime_error, std::unique_ptr;
using std::ranges::equal;
using StreamUtils::read;

namespace JFIF
{
PayloadSegment::PayloadSegment(istream& input)
{
	length = read<big_uint16_t>(input);
}

App0Segment::App0Segment(istream& input):
    PayloadSegment(input)
{
	if (length < 16)
	{
		throw runtime_error { "segment APP0 too short" };
	}

	array<char, 5> identifier;
	input.read(identifier.data(), 5);
	if (!equal(identifier, "JFIF"))
	{
		throw runtime_error { "JFIF marker missing or incorrect" };
	}

	version         = read<big_uint16_t>(input);
	units           = static_cast<Units>(big_to_native(read<uint8_t>(input)));
	xDensity        = read<big_uint16_t>(input);
	yDensity        = read<big_uint16_t>(input);
	thumbnailWidth  = read<big_uint8_t>(input);
	thumbnailHeight = read<big_uint8_t>(input);
	// TODO: impl thumbnail I guess
}

static auto segmentFactory(SegmentType type, istream& input) -> unique_ptr<Segment>
{
	using P = unique_ptr<Segment>;

	switch (type)
	{
	case SegmentType::APP0:
		return P { new App0Segment(input) };

	default:
		return P { new Segment };
	}
}

auto readSegment(istream& input) -> unique_ptr<Segment>
{
	if (input.peek() != 0xFF)
	{
		throw runtime_error { "invalid segment marker (not 0xFF)" };
	}

	int byte;
	do
	{
		byte = input.get();
	} while (byte == 0xFF);

	if (byte == istream::traits_type::eof())
	{
		throw runtime_error { "expected segment, got EOF" };
	}

	const auto type = static_cast<SegmentType>(byte);
	auto segment    = segmentFactory(type, input);
	segment->type   = type;
	segment->offset = input.tellg();
	return segment;
}

}