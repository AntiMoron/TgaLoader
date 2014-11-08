#ifndef _TGALOADER_HPP__
#define _TGALOADER_HPP__

#include<cstdio>
#include<string>
#include<memory>
#include<exception>
#include<algorithm>
#include"TgaData.hpp"

namespace TGA
{
	class TgaLoader
	{
	public:
		TgaLoader()
		{
		}
		~TgaLoader()
		{
		}

		//make sure your program is going to exit or you don't need to load any tga anymore.
		static void Shutdown()
		{
			if(nullptr != instance)
			{
				delete instance;
				instance = nullptr;
			}
		}

        //Use this method to get a tgaLoader
		static TgaLoader& getLoader()
		{
			if(nullptr == instance)
			{
				instance = new TgaLoader;
			}
			return (*instance);
		}
		//LoadTgaFile
		bool loadFile(const char* filename,TgaData* pOut)
		{
			if(isSuffix(filename,".tga") == false && isSuffix(filename,".TGA") == false)
			{
				printf("can't open file '%s' at 'LoadFile' function.\r\n",filename);
				throw bad_file_exception("this is not a .tga file.\r\n");
				return false;
			}

			FILE* file_ptr = nullptr;
			file_ptr = fopen(filename,"rb");
			if(nullptr == file_ptr)
			{
				printf("can't open file '%s' at 'LoadFile' function.\r\n",filename);
				throw bad_file_exception("file not exist");
				return false;
			}

			unsigned char temData[18];
			fread(&temData,sizeof(unsigned char),18,file_ptr);
			pOut->id = temData[0];
			pOut->colorMapType = temData[1];
			pOut->imageType = temData[2];
			//color map specification
			pOut->firstEntryIndex = *((unsigned short*)&temData[3]);
			pOut->colorMapLenth = *((unsigned short*)&temData[5]);
			pOut->colorMapEntrySize = temData[7];
			//image specification
			pOut->xOrigin = *((unsigned short*)&temData[8]);
			pOut->yOrigin = *((unsigned short*)&temData[10]);
			pOut->width = *((unsigned short*)&temData[12]);
			pOut->height = *((unsigned short*)&temData[14]);
			pOut->pixelDepth = temData[16];
			pOut->imageDescriptor = temData[17];

			printf("id:%d\n"
					"colorMapType:%d\n"
					"imageType:%d\n"
					"ColorMapSpecification:\n"
					"\tfirst entry index:%d\n"
					"\tcolor map lenth:%d\n"
					"\tcolor map Entry Size:%d\n"
					"Image Specification\n"
					"\tX-origin of Image:%d\n"
					"\tY-origin of Image:%d\n"
					"\tImage Width : %d\n"
					"\tImage Height : %d\n"
					"\tPixel Depth : %d\n"
					"\tImage Descriptor : %d\n",
					pOut->id,
					pOut->colorMapType,
					pOut->imageType,
					pOut->firstEntryIndex,
					pOut->colorMapLenth,
					pOut->colorMapEntrySize,
					pOut->xOrigin,
					pOut->yOrigin,
                    pOut->width,
                    pOut->height,
                    pOut->pixelDepth,
                    pOut->imageDescriptor
					);

			if(pOut->id != 0)
			{
				printf("Image Ids:\n");
				unsigned char* imageId = new unsigned char[pOut->id];
				fread(imageId,sizeof(unsigned char),pOut->id,file_ptr);
				for(int i=0;i<pOut->id;i++)
				{
					printf("%d ",imageId[i]);
				}
				if(imageId != nullptr)
				{
					delete[] imageId;
					imageId = nullptr;
				}
			}
			else
			{
				printf("No Image Id.\n");
			}
			// in the test tga file I used ,there's no color map.
			//So I didn't process the color map data temporarily.
			if(pOut->colorMapType != 0)
			{
				int colorBitField = std::min(pOut->colorMapEntrySize / 3 ,8);
			}
			else
			{
				printf("There is no color map.\n");
			}

			short bytesPerPixel = pOut->pixelDepth / 8;
			std::size_t pixelCount = (unsigned long)(pOut->width) * (unsigned long)(pOut->height);
			std::size_t colorDataFieldLenth = pixelCount * bytesPerPixel;
			unsigned char* colorData = new unsigned char[colorDataFieldLenth];
			pOut->pColor = new color4i[pixelCount];
			printf("%d\n",colorDataFieldLenth);
			fread(colorData,sizeof(unsigned char),colorDataFieldLenth,file_ptr);
			// read the true-type image data.
			for(int i = 0;i < pixelCount;i++)
			{
				pOut->pColor[i].a = colorData[bytesPerPixel * i];
				if(bytesPerPixel > 1)
					pOut->pColor[i].r = colorData[bytesPerPixel * i + 1];
				else
					pOut->pColor[i].r = 0.0f;
				if(bytesPerPixel > 2)
					pOut->pColor[i].g = colorData[bytesPerPixel * i + 2];
				else
					pOut->pColor[i].g = 0.0f;
				if(bytesPerPixel > 3)
					pOut->pColor[i].b = colorData[bytesPerPixel * i + 3];
				else
					pOut->pColor[i].b = 0.0f;
//				printf("%d %d %d %d\n",pOut->pColor[i].r,pOut->pColor[i].g,pOut->pColor[i].b,pOut->pColor[i].a);
			}
			if(colorData != nullptr)
			{
				delete [] colorData;
				colorData = nullptr;
			}

			if(nullptr != file_ptr)
			{
				fclose(file_ptr);
				file_ptr = nullptr;
			}
			return true;
		}

		class bad_file_exception : public std::exception
		{
		public:
			bad_file_exception(const char* err_str = nullptr)
			{
				if(nullptr == err_str)
				{
					error_msg = "bad_file_exception.\r\n";
					return ;
				}
				error_msg = err_str;
			}
			~bad_file_exception(){}

			const char* what() const noexcept
			{
				return error_msg.c_str();
			}
		private:
			std::string error_msg;
		};

	private:
		//judge string@suf whether is the suffix of string@src
		bool isSuffix(const std::string& src,const std::string& suf)
		{
			size_t lenSrc = src.length();
			size_t lenSuf = suf.length();
			if(lenSrc < lenSuf)
				return false;
			if(lenSrc == lenSuf)
				return src == suf;
			//"abcdef" "abc"
			size_t sufCur = 0;
			for(size_t srcCur = lenSrc - lenSuf;srcCur < lenSrc;++srcCur,++sufCur)
			{
				if(src[srcCur] != suf[sufCur])
				{
					return false;
				}
			}
			return true;
		}

		static TgaLoader* instance;
	};
}

#define TGALOAD(path,out) (TGA::TgaLoader::getLoader().loadFile(path,out))
#endif // _TGALOADER_H__
