#include "JfifFile.hpp"

#include "Segment.hpp"

using std::istream;

namespace JFIF
{
JfifFile::JfifFile(istream& stream):
    _stream { stream }
{
	const auto soi = readSegment(stream);
	if (soi->type != SegmentType::SOI)
	{	// TODO handle
		return;
	}

	const auto app0 = readSegment(stream);
	if (app0->type != SegmentType::APP0)
	{	// TODO handle
		return;
	}
}
}
