#include"tgaLoader.hpp"

int main()
{
	TGA::TgaLoader::Initialize();
	printf(TGA::TgaLoader::isInited()?
	"TgaLoader Initialize successfully.\r\n"
	:"TgaLoader Initialization failed.\r\n");

	TGA::TgaData m;
	try
	{
		TGA::TgaLoader::GetLoader().LoadFile("tgaTest.tga",&m);
		printf("Succeed so far.\r\n");
	}
	catch(const std::exception& e)
	{
		printf("%s\r\n",e.what());
	}
	return 0;
}
