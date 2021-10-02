#include "JfifFile.hpp"

using std::istream;

namespace JFIF
{
JfifFile::JfifFile(istream& stream):
    _stream { stream }
{
}
}
