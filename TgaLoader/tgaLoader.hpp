#ifndef _TGALOADER_HPP__
#define _TGALOADER_HPP__

#include<cstdio>
#include<string>
#include<exception>
#include"TgaData.hpp"

namespace TGA
{
	typedef unsigned char BYTE;
	typedef unsigned long ULONG;
	typedef unsigned short USHORT;
	class TgaLoader
	{
	public:
		TgaLoader()
		{
		}
		~TgaLoader()
		{
			;
		}

		//make sure this function is called on your program's initialization.
		static bool Initialize()
		{
			b_isInited = false;

			instance = nullptr;

			instance = new TgaLoader;
			if(nullptr == instance)
			{
				return b_isInited;
			}

			b_isInited = true;
			return b_isInited;
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
		static TgaLoader& GetLoader()
		{
			return (*instance);
		}
		//LoadTgaFile
		bool LoadFile(const char* filename,TgaData* pOut)
		{
			if(isSuffix(filename,".tga") == false && isSuffix(filename,".TGA") == false)
			{
				printf("can't open file '%s' at 'LoadFile' function.\r\n",filename);
				throw bad_file_exception("this is not a .tga file.\r\n");
				return false;
			}

			FILE* file_ptr = nullptr;
			file_ptr = fopen(filename,"r");
			if(nullptr == file_ptr)
			{
				printf("can't open file '%s' at 'LoadFile' function.\r\n",filename);
				throw bad_file_exception("file not exist");
				return false;
			}

			if(nullptr != file_ptr)
			{
				fclose(file_ptr);
				file_ptr = nullptr;
			}
			return true;
		}

		static bool isInited()
		{
			return b_isInited;
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
			~bad_file_exception()
			{
				;
			}

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

		static bool b_isInited;
		static TgaLoader* instance;
	};
}


#endif // _TGALOADER_H__
