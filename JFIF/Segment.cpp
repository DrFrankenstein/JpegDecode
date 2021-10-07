#include "Segment.hpp"

#include "../StreamUtils/readers.hpp"

#include <algorithm>
#include <array>
#include <boost/endian/arithmetic.hpp>
#include <boost/endian/conversion.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <span>
#include <utility>

using boost::endian::big_to_native, boost::endian::big_uint8_t, boost::endian::big_uint16_t;
using std::array, std::byte, std::istream, std::move, std::pair, std::runtime_error, std::span, std::unique_ptr;
using std::ranges::copy, std::ranges::equal;
using StreamUtils::read, StreamUtils::readIntoBuffer;

namespace JFIF
{
template<typename To, typename From>
auto uniquePtrStaticCast(unique_ptr<From> source)
{
	const auto sourcePtr = source.release();
	return unique_ptr<To>(static_cast<To*>(sourcePtr));
}

template<typename T>
auto podMakeUnique()
{  // std::make_unique requires a constructor,
	// and typing the typename twice sucks. :(
	return unique_ptr<T> { new T };
}

static auto readJfifSegment(const byte* payload, uint16_t length)
{
#pragma pack(push, 1)
	struct JfifRecord
	{
		array<char, 5> identifier;
		big_uint16_t version;
		big_uint8_t units;
		big_uint16_t xDensity;
		big_uint16_t yDensity;
		big_uint8_t xThumbnail;
		big_uint8_t yThumbnail;
		// thumbnail RGB[n] read separately
	};
#pragma pack(pop)

	if (length < sizeof(JfifRecord))
	{
		throw runtime_error { "JFIF segment too short" };
	}

	const auto record = reinterpret_cast<const JfifRecord*>(payload);

	auto segment  = podMakeUnique<JfifSegment>();
	segment->type = SegmentType::APP0;
	copy(record->identifier, segment->identifier.begin());
	segment->version         = record->version;
	segment->units           = static_cast<JfifSegment::Units>(uint8_t { record->units });
	segment->xDensity        = record->xDensity;
	segment->yDensity        = record->yDensity;
	segment->thumbnailWidth  = record->xThumbnail;
	segment->thumbnailHeight = record->yThumbnail;
	// TODO: copy and handle thumbnail

	return segment;
}

static auto readJfxxSegment(const byte* payload, uint16_t length)
{
#pragma pack(push, 1)
	struct JfxxRecord
	{
		array<char, 5> identifier;
		big_uint8_t extensionCode;
	};
#pragma pack(pop)

	if (length < sizeof(JfxxRecord))
	{
		throw runtime_error { "JFXX segment too short" };
	}

	const auto record = reinterpret_cast<const JfxxRecord*>(payload);

	auto segment  = podMakeUnique<JfxxSegment>();
	segment->type = SegmentType::APP0;
	copy(record->identifier, segment->identifier.begin());
	segment->extensionCode = record->extensionCode;
	// TODO: implement rest

	return segment;
}

auto readPayloadSegment(istream& input)
{
	const uint16_t length = read<big_uint16_t>(input) - sizeof length;  // length counts itself in the file
	auto payload          = readIntoBuffer<const byte>(input, length);
	return pair { length, move(payload) };
}

auto readApp0Segment(istream& input) -> unique_ptr<App0Segment>
{
	auto [length, payload] = readPayloadSegment(input);

	if (length < 5)
	{
		throw runtime_error { "APP0 segment too short" };
	}

	span<const char> identifier { reinterpret_cast<const char*>(payload.get()), 5 };
	if (equal(identifier, "JFIF"))
	{
		return readJfifSegment(payload.get(), length);
	}
	if (equal(identifier, "JFXX"))
	{
		return readJfxxSegment(payload.get(), length);
	}

	// type unsupported
	return {};
}

static auto segmentFactory(SegmentType type, istream& input) -> unique_ptr<Segment>
{
	switch (type)
	{
	case SegmentType::APP0:
		return readApp0Segment(input);

	default: {
		auto segment = podMakeUnique<Segment>();
		segment->type = type;
		return segment;
	}
	}
}

auto readSegment(istream& input) -> unique_ptr<Segment>
{
	if (input.peek() != 0xFF)
	{
		throw runtime_error { "invalid segment marker (not 0xFF)" };
	}

	const auto offset = input.tellg();

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
	segment->offset = offset;
	return segment;
}

}
