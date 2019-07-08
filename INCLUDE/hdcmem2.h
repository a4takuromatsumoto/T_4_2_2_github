#ifndef HDCMEM
#define HDCMEM
#include <windows.h>
#pragma warning(disable : 4309)
#pragma warning(disable : 4267)

class hdcmem{
	typedef char byte;
public:
	HDC h;
	HPEN hPen;
	HBRUSH hBrush;
	HBITMAP hBitmap;
	struct _size{int x, y;}size;
	hdcmem(HWND hWnd){
		RECT rect;
		GetWindowRect(hWnd, &rect);
		size.x = rect.right - rect.left;
		size.y = rect.bottom - rect.top;
		h = CreateCompatibleDC(NULL);
		SelectObject(h, hBitmap = CreateCompatibleBitmap(GetDC(hWnd), size.x, size.y));
		SelectObject(h, hBrush = CreateSolidBrush(RGB(255, 255, 255)));
		SelectObject(h, hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255)));
		SetBkMode(h, TRANSPARENT);
		settextcolor(255, 255, 255);
		PatBlt(h, 0, 0, size.x, size.y, BLACKNESS);
	}
	hdcmem(int x, int y){
		size.x = x;
		size.y = y;
		h = CreateCompatibleDC(NULL);
		SelectObject(h, hBitmap = CreateCompatibleBitmap(GetDC(NULL), size.x, size.y));
		SelectObject(h, hBrush = CreateSolidBrush(RGB(255, 255, 255)));
		SelectObject(h, hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255)));
		SetBkMode(h, TRANSPARENT);
		settextcolor(255, 255, 255);
		PatBlt(h, 0, 0, size.x, size.y, BLACKNESS);
	}
	~hdcmem(){
		DeleteObject(h);
		DeleteObject(hBitmap);
		DeleteObject(hBrush);
		DeleteObject(hPen);
	}
	HPEN selectpen(byte r, byte g, byte b){
		HPEN hPen2= CreatePen(PS_SOLID, 1, RGB(r, g, b));
		SelectObject(h, hPen2);
		return hPen2;
	}
	HPEN selectpen(byte r, byte g, byte b, int i){
		HPEN hPen2= CreatePen(PS_SOLID, i, RGB(r, g, b));
		SelectObject(h, hPen2);
		return hPen2;
	}
	HFONT selectfont(int scale){
		HFONT hFont = CreateFont(scale, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"ÇlÇrÅ@ñæí©");
		SelectObject(h, hFont);
		return hFont;
	}
	HBRUSH selectbrush(byte r, byte g, byte b){
		HBRUSH hBrush2 = CreateSolidBrush(RGB(r, g, b));
		SelectObject(h, hBrush2);
		return hBrush2;
	}
	HBRUSH selectnullbrush(){
		HBRUSH hBrush2 = (HBRUSH)GetStockObject(NULL_BRUSH);
		SelectObject(h, hBrush2);
		return hBrush2;
	}
	void settextcolor(byte r, byte g, byte b){SetTextColor(h, RGB(r, g, b));}
	void copyto(HDC hdc){BitBlt(hdc, 0, 0, size.x, size.y, h, 0, 0, SRCCOPY);}
	void copyto(HDC hdc, int x, int y){BitBlt(hdc, x, y, size.x, size.y, h, 0, 0, SRCCOPY);}
	void copyfrom(HDC hdc, int x, int y){BitBlt(h, x, y, size.x, size.y, hdc, 0, 0, SRCCOPY);}
	void line(int x, int y, int x2, int y2){
		MoveToEx(h, x, y, NULL);
		LineTo(h, x2, y2);
	}
	void fillrect(int x, int y, int x2, int y2){Rectangle(h, x, y, x2, y2);}
	void fillrect(int x, int y, int x2, int y2, byte r, byte g, byte b){
		HPEN hPen = selectpen(r, g, b);
		HBRUSH hBrush = selectbrush(r, g, b);
		Rectangle(h, x, y, x2, y2);
		DeleteObject(hBrush);
		DeleteObject(hPen);
	}
	void rect(int x, int y, int x2, int y2){
		HBRUSH hBrush2 = (HBRUSH)GetStockObject(NULL_BRUSH);
		SelectObject(h, hBrush2);
		fillrect(x, y, x2, y2);
		DeleteObject(hBrush2);
	}
	void rect_white(){
		HPEN hPen2 = selectpen(255, 255, 255);
		rect(0, 0, size.x, size.y);
		DeleteObject(hPen2);
	}
	void gradrect(int x, int y, int x2, int y2, int r_top, int g_top, int b_top, int r_bottom, int g_bottom, int b_bottom){
		HPEN hPen;
		for(int ai = 0; ai < y2 - y; ai++){
			hPen = selectpen(r_top + (int)((double)ai * (double)(r_bottom - r_top) / (double)(y2 - y)),
							g_top + (int)((double)ai * (double)(g_bottom - g_top) / (double)(y2 - y)),
							b_top + (int)((double)ai * (double)(b_bottom - b_top) / (double)(y2 - y)));
			line(x, y + ai, x2, y + ai);
			DeleteObject(hPen);
		}
	}
	void pset(int x, int y){SetPixel(h, x, y, RGB(255, 255, 255));}
	void pset(int x, int y, byte r, byte g, byte b){SetPixel(h, x, y, RGB(r, g, b));}
	int getrp(int x, int y){
		COLORREF cr = GetPixel(h, x, y);
		return GetRValue(cr);
	}
	int getgp(int x, int y){
		COLORREF cr = GetPixel(h, x, y);
		return GetGValue(cr);
	}
	int getbp(int x, int y){
		COLORREF cr = GetPixel(h, x, y);
		return GetBValue(cr);
	}
	void polygon(int x, int y, int x2, int y2, int x3, int y3){
		POINT pt[3] = {{x, y}, {x2, y2}, {x3, y3}};
		Polygon(h, pt, 3);
	}
	BOOL circle(int x, int y, int r){return Ellipse(h, x - r, y - r, x + r, y + r);}
	void string(int x, int y, LPCWSTR c){TextOut(h, x, y, (LPCWSTR)c, wcslen(c));}
	void string(int x, int y, int i){
		char c[256];
		wsprintf((LPWSTR)c, L"%d", i);
		string(x, y, (LPCWSTR)c);
	}
	void string(int x, int y, double d){
		char c[256];
		wsprintf((LPWSTR)c, L"%f", d);
		string(x, y, (LPCWSTR)c);
	}
	void flush(){PatBlt(h, 0, 0, size.x, size.y, BLACKNESS);}
	void flush_white(){PatBlt(h, 0, 0, size.x, size.y, WHITENESS);}
	void invalidate(HWND hWnd){InvalidateRect(hWnd, NULL, FALSE);}
	byte d_plus(double d){
		if(d < 0)
			return 0;
		else if(d >= 1)
			return 255;
		return (byte)(d * 256);
	}
	void copy_from_screen(){
		HDC hdc = GetDC(NULL);
		HDC hDest = CreateCompatibleDC(hdc);

		int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);

		HBITMAP hbDesktop = CreateCompatibleBitmap(hdc, width, height);

		SelectObject(hDest, hbDesktop);

		BitBlt(hDest, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

		BitBlt(h, 0, 0, width, height, hDest, 0, 0, SRCCOPY);

		ReleaseDC(NULL, hdc);
	}
};
#endif