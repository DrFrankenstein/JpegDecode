module JfifFile;

import <iostream>;

using std::istream;

namespace JFIF
{
JfifFile::JfifFile(istream& stream):
    _stream { stream }
{
}
}
