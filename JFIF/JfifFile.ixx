export module JFIF.JfifFile;

import std.core;

using std::istream;

export namespace JFIF
{
class JfifFile
{
	public:
	JfifFile(istream& stream);

	private:
	istream& _stream;
};
}