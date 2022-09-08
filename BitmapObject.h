# pragma once
#include <Windows.h>

class BitMapObject {
private:
	//memory hdc
	HDC hdcMemory;
	//new bitmap
	HBITMAP hbmNewBitMap;
	//old bitmap
	HBITMAP hbmOldBitMap;
	//width & height as integers
	int iWidth;
	int iHeight;

public:
	//constructor
	BitMapObject();
	//destructor
	~BitMapObject();

	//loads bitmap from file
	void Load(HDC hdcCompatible, LPCWSTR lpszFilename);

	//creates a blank bitmap
	void Create(HDC hdcCompatible, int width, int height);

	//destroys bitmap and dc
	void Destroy();

	//return width
	int GetWidth();

	//return height
	int GetHeight();

	//converts to HDC
	operator HDC();

};
