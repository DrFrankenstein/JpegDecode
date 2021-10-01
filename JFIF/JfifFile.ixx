export module JfifFile;

import<iostream>;

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