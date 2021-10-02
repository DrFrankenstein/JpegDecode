module JFIF.JfifFile;

import std.core;

using std::istream;

namespace JFIF
{
JfifFile::JfifFile(istream& stream):
    _stream { stream }
{
}
}
