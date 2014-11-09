#include"tgaLoader.hpp"
#include<iostream>
#include<windows.h>
using namespace std;

HWND hwnd;
HDC hdc;
RECT g_rect;
HDC buffer;
HDC g_bitmap;
HBITMAP bmp;

int pixDraw(HDC _hdc,int x,int y,unsigned char r,unsigned char g,unsigned char b)
{
	SelectObject(_hdc,bmp);
	HPEN p = CreatePen(PS_SOLID,1,RGB(r,g,b));
	HBRUSH p2 = CreateSolidBrush(RGB(r,g,b));
	SelectObject(_hdc,p);
	SelectObject(_hdc,p2);
	RECT rect;
	rect.left = x;
	rect.right = x + 1;
	rect.top = y;
	rect.bottom = y + 1;
	FillRect(_hdc,&rect,p2);
	DeleteObject(p);
	DeleteObject(p2);
	return 0;
}


int main()
{
	SetConsoleTitle("Test");
	while((hwnd = FindWindow(NULL,"Test"))==NULL);
	hdc = GetDC(hwnd);
	GetClientRect(hwnd,&g_rect);
	buffer = CreateCompatibleDC(hdc);
	g_bitmap = CreateCompatibleDC(hdc);
	bmp = CreateCompatibleBitmap(hdc,g_rect.right,g_rect.bottom);;

	TGA::TgaData m;
	try
	{
//		tgaTest.tga
		TGA::TgaLoader::getLoader().loadFile("tgaTest.tga",&m);
		printf("Succeed so far.\r\n");
		for(int i=0;i<m.getHeight();i++)
		{
			for(int j=0;j<m.getWidth();j++)
			{
				auto& color = m.getColDataPtr()[i * m.getWidth() + j];
				pixDraw(buffer,j,i,color.r,color.g,color.b);
			}
		}
	}
	catch(const std::exception& e)
	{
		printf("%s\r\n",e.what());
	}
	BitBlt(hdc,0,0,g_rect.right - g_rect.left,g_rect.bottom - g_rect.top,buffer,0,0,SRCCOPY);
	return 0;
}
