#ifndef MINE_AVI
#define MINE_AVI

#include <math.h>
#include <tchar.h>
#include <windows.h>
#include <vfw.h>
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "Vfw32.lib")

#define PI 3.1415926535

class avi{
private:
	int w, h;
	int i, j, s;
	int count;
	AVISTREAMINFO si;
	BITMAPINFOHEADER bmih;
	BYTE *bBit;
	PAVIFILE pavi;
	PAVISTREAM pstm;
public:
	/*
	avi(char *pname, int width, int height){
		w = width;
		h = height;
		AVISTREAMINFO si2 = {streamtypeVIDEO, comptypeDIB, 0, 0, 0, 0, 1, 30, 0, w, 0, 0, (DWORD)-1, 0, 
			{0, 0, w, h}, 0, 0, _T("Video #1")};
		
		BITMAPINFOHEADER bmih2= {
			sizeof(BITMAPINFOHEADER),
				w,
				h,
				1,
				24,
				BI_RGB,
				width * height * 3,
				0, 0, 0, 0};
		memcpy(&si, &si2, sizeof(si));
		memcpy(&bmih, &bmih2, sizeof(bmih));
		count = 0;
		AVIFileInit();
		if(AVIFileOpen(&pavi, _T(pname), OF_CREATE | OF_WRITE | OF_SHARE_DENY_NONE, NULL))
			return;
		if(AVIFileCreateStream(pavi, &pstm, &si) != 0)
			return;
		if(AVIStreamSetFormat(pstm, 0, &bmih, sizeof(BITMAPINFOHEADER)) != 0)
			return;
	}
	*/
	avi(wchar_t *pname, int width, int height){
		w = width;
		h = height;
		AVISTREAMINFO si2 = {streamtypeVIDEO, comptypeDIB, 0, 0, 0, 0, 1, 30, 0, w, 0, 0, (DWORD)-1, 0, 
			{0, 0, w, h}, 0, 0, _T("Video #1")};
		
		BITMAPINFOHEADER bmih2= {
			sizeof(BITMAPINFOHEADER),
				w,
				h,
				1,
				24,
				BI_RGB,
				width * height * 3,
				0, 0, 0, 0};
		memcpy(&si, &si2, sizeof(si));
		memcpy(&bmih, &bmih2, sizeof(bmih));
		count = 0;
		AVIFileInit();
		if(AVIFileOpen(&pavi, pname, OF_CREATE | OF_WRITE | OF_SHARE_DENY_NONE, NULL))
			return;
		if(AVIFileCreateStream(pavi, &pstm, &si) != 0)
			return;
		if(AVIStreamSetFormat(pstm, 0, &bmih, sizeof(BITMAPINFOHEADER)) != 0)
			return;
	}
	void write(byte *bBit){
		if(AVIStreamWrite(pstm, count, 1, bBit, bmih.biSizeImage, AVIIF_KEYFRAME, NULL, NULL) != 0)
			return;
		count++;
	}
	void save(){
		AVIStreamRelease(pstm);
		AVIFileRelease(pavi);
		AVIFileExit();
	}
};
#endif