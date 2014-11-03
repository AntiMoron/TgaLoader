#include"tgaLoader.hpp"

int main()
{
	TGA::TgaData m;
	try
	{
		TGA::TgaLoader::getLoader().loadFile("tgaTest.tga",&m);
		printf("Succeed so far.\r\n");
	}
	catch(const std::exception& e)
	{
		printf("%s\r\n",e.what());
	}
	return 0;
}
