import std.core;  // breaks Intellisense :( <https://developercommunity2.visualstudio.com/t/Intellisense-lacks-support-for-C-Stand/1317270>

import JFIF;

using JFIF::JfifFile;

int main(int argc, char* argv[])
{
	if (argc < 2)
		std::exit(1);

	std::ifstream stream { argv[1] };
	JfifFile file { stream };
}