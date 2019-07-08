#ifndef MINE_WINDOW
#define MINE_WINDOW
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <shellapi.h>
#include <stdlib.h>
#include "INCLUDE/hdcmem2.h"

#define WIDTH_WINDOW 600
#define HEIGHT_WINDOW 400
#define WM_INIT WM_USER
#define WM_EDITRETURN WM_USER+1
#define WM_EDITRETURN2 WM_USER+2
#define WM_LMLBUTTONUP WM_USER+3
#define WM_LVDBLCLK WM_USER+4
#define WM_TBCHANGE WM_USER+5
#define WM_QUEUEMESSAGE WM_USER+6
#define WM_USER2 WM_USER+7
#define WM_LMLBUTTONDBLCLK WM_USER+8
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)
#pragma warning(disable : 4355)
#pragma comment(lib, "comctl32.lib")
class window{
public:
	typedef char byte;
	struct _color{byte r, g, b;};
	struct _wcolor{_color back, pen;};
	char cname[256];
	void *pv;
	struct _arg{
		HWND hwnd;
		UINT uMsg;
		WPARAM wp;
		LPARAM lp;
	};
	struct _procedure{
		HBRUSH hBrush_edit;
		hdcmem *phm;
		bool b_ldown, b_rdown;
		bool b_return;
		LRESULT lr_return;
		HWND hWnd;
		window *pw;
		HINSTANCE hInst;
		struct _gui{
			HWND h;
			void setdata(){SetWindowLong(h, GWL_USERDATA, (LONG)this);}
			void invalidate(){InvalidateRect(h, NULL, FALSE);}
			void show(){ShowWindow(h, SW_SHOWNORMAL);}
			void hide(){ShowWindow(h, SW_HIDE);}
		};
		struct _button : public _gui{
			HDC hdc_normal, hdc_selected;
			_button(_procedure *p_proc, wchar_t *name, int x, int y, int w, int h, int id){
				this->h = CreateWindow(L"BUTTON", (LPCWSTR)name, WS_VISIBLE | WS_CHILD,
					x, y, w, h, p_proc->hWnd, (HMENU)id, NULL, 0);
				setfont(this->h, 12);
				setdata();
			}
			_button(_procedure *p_proc, wchar_t *name, int x, int y, int w, int h, int id, HDC hdc_normal, HDC hdc_selected){
				this->h = CreateWindow(L"BUTTON", (LPCWSTR)name, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
					x, y, w, h, p_proc->hWnd, (HMENU)id, NULL, 0);
				setfont(this->h, 12);
				setdata();
				this->hdc_normal = hdc_normal;
				this->hdc_selected = hdc_selected;
			}
			void set_hdc_normal(HDC hdc_normal){
				DeleteObject(this->hdc_normal);
				this->hdc_normal = hdc_normal;
			}
			void set_hdc_selected(HDC hdc_selected){
				DeleteObject(this->hdc_selected);
				this->hdc_selected = hdc_selected;
			}
		};
		struct _radiobutton : public _gui{
			_radiobutton(_procedure *p_proc, int x, int y, int w, int h, int id){
				this->h = CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
					x, y, w, h, p_proc->hWnd, (HMENU)id, NULL, 0);
				setdata();
			}
			void set(bool b){b ? SendMessage(h, BM_SETCHECK, BST_CHECKED, 0) : SendMessage(h, BM_SETCHECK, BST_UNCHECKED, 0);}
			bool get(){return (SendMessage(h, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;}
			void reverse(){set(!get());}
		};
		struct _radiobutton2 : public _gui{
			_radiobutton2(_procedure *p_proc, wchar_t *pc, int x, int y, int w, int h, int id, bool b_start){
				this->h = CreateWindow(L"BUTTON", (LPCWSTR)pc, WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | (b_start ? WS_GROUP : NULL),
					x, y, w, h, p_proc->hWnd, (HMENU)id, NULL, 0);
				setdata();
			}
			void set(bool b){b ? SendMessage(h, BM_SETCHECK, BST_CHECKED, 0) : SendMessage(h, BM_SETCHECK, BST_UNCHECKED, 0);}
			bool get(){return (SendMessage(h, BM_GETCHECK, 0, 0) == BST_CHECKED) ? true : false;}
			void reverse(){set(!get());}
		};
		struct _staticwindow : public _gui{
			hdcmem *phm;
			FARPROC OldStaticProc;
			int lx, ly, lw, lh;
			struct _mouse{
				virtual void mousemove(int x, int y){}
			}*pm;
			_staticwindow(_procedure *p_proc, int x, int y, int w, int h){
				this->h = CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD,
					x, y, w, h, p_proc->hWnd, 0, NULL, 0);
				setdata();
				phm = new hdcmem(w, h);
				phm->flush();
				pm = NULL;
				this->lx = x;
				this->ly = y;
				this->lw = w;
				this->lh = h;
				OldStaticProc = (FARPROC)GetWindowLong(this->h, GWL_WNDPROC);
				SetWindowLong(this->h, GWL_WNDPROC, (LONG)NewStaticProc);
			}
			_staticwindow(_procedure *p_proc, int x, int y, int w, int h, _mouse *pm){
				this->h = CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD,
					x, y, w, h, p_proc->hWnd, 0, NULL, 0);
				setdata();
				phm = new hdcmem(w, h);
				phm->flush();
				this->pm = pm;
				this->lx = x;
				this->ly = y;
				this->lw = w;
				this->lh = h;
				OldStaticProc = (FARPROC)GetWindowLong(this->h, GWL_WNDPROC);
				SetWindowLong(this->h, GWL_WNDPROC, (LONG)NewStaticProc);
			}
			_staticwindow(_procedure *p_proc, char *name, int x, int y, int w, int h){
				this->h = CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD,
					x, y, w, h, p_proc->hWnd, 0, NULL, 0);
				setdata();
				phm = new hdcmem(w, h);
				phm->flush();
				HFONT hFont = phm->selectfont(12);
				phm->string(0, 0, (LPCWSTR)name);
				DeleteObject(hFont);
				pm = NULL;
				this->lx = x;
				this->ly = y;
				this->lw = w;
				this->lh = h;
				OldStaticProc = (FARPROC)GetWindowLong(this->h, GWL_WNDPROC);
				SetWindowLong(this->h, GWL_WNDPROC, (LONG)NewStaticProc);
			}
			~_staticwindow(){
				if(pm)
					delete pm;
			}
			bool calc_mouse(int x, int y, int &x2, int &y2){
				x2 = x - this->lx;
				y2 = y - this->ly;
				if(x2 >= 0 && x2 < lw && y2 >= 0 && y2 < lh)
					return true;
				return false;
			}
			static LRESULT CALLBACK NewStaticProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp){
				_staticwindow *psw = (_staticwindow *)GetWindowLong(hWnd, GWL_USERDATA);
				switch(uMsg){
					case WM_PAINT:
						{
							PAINTSTRUCT ps;
							HDC hdc = BeginPaint(hWnd, &ps);
							psw->phm->copyto(hdc);
							EndPaint(hWnd, &ps);
						}
						break;
					case WM_NCHITTEST:
						if(psw->pm)
							psw->pm->mousemove(LOWORD(lp), HIWORD(lp));
						break;
				}
				return CallWindowProc((WNDPROC)psw->OldStaticProc, hWnd, uMsg, wp, lp);
			}
		};
		struct _staticwindow2 : public _gui{
			hdcmem *phm;
			FARPROC OldStaticProc;
			struct _mouse{
				bool b_ldown, b_rdown;
				_mouse(){
					b_ldown = false;
					b_rdown = false;
				}
				virtual void mousemove(int x, int y){}
				virtual void lbuttondown(int x, int y){}
				virtual void lbuttonup(int x, int y){}
				virtual void rbuttondown(int x, int y){}
				virtual void rbuttonup(int x, int y){}
			}*pm;
			_staticwindow2(_procedure *p_proc, int x, int y, int w, int h){
				this->h = CreateWindow(L"static2", L"", WS_VISIBLE | WS_CHILD,
					x, y, w, h, p_proc->hWnd, 0, NULL, 0);
				setdata();
				phm = new hdcmem(w, h);
				phm->flush();
				pm = NULL;
				OldStaticProc = (FARPROC)GetWindowLong(this->h, GWL_WNDPROC);
				SetWindowLong(this->h, GWL_WNDPROC, (LONG)NewStatic2Proc);
			}
			_staticwindow2(_procedure *p_proc, int x, int y, int w, int h, _mouse *pm){
				this->h = CreateWindow(L"static2", L"", WS_VISIBLE | WS_CHILD,
					x, y, w, h, p_proc->hWnd, 0, NULL, 0);
				setdata();
				phm = new hdcmem(w, h);
				phm->flush();
				this->pm = pm;
				OldStaticProc = (FARPROC)GetWindowLong(this->h, GWL_WNDPROC);
				SetWindowLong(this->h, GWL_WNDPROC, (LONG)NewStatic2Proc);
			}
			~_staticwindow2(){
				if(pm)
					delete pm;
			}
			static LRESULT CALLBACK NewStatic2Proc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp){
				_staticwindow2 *psw2 = (_staticwindow2 *)GetWindowLong(hWnd, GWL_USERDATA);
				switch(uMsg){
					case WM_PAINT:
						{
							PAINTSTRUCT ps;
							HDC hdc = BeginPaint(hWnd, &ps);
							psw2->phm->copyto(hdc);
							EndPaint(hWnd, &ps);
						}
						break;
					case WM_MOUSEMOVE:
						if(psw2->pm)
							psw2->pm->mousemove(LOWORD(lp), HIWORD(lp));
						break;
					case WM_LBUTTONDOWN:
						if(psw2->pm){
							psw2->pm->b_ldown = true;
							psw2->pm->lbuttondown(LOWORD(lp), HIWORD(lp));
						}
						break;
					case WM_LBUTTONUP:
						if(psw2->pm){
							psw2->pm->b_ldown = false;
							psw2->pm->lbuttonup(LOWORD(lp), HIWORD(lp));
						}
						break;
					case WM_RBUTTONDOWN:
						if(psw2->pm){
							psw2->pm->b_rdown = true;
							psw2->pm->rbuttondown(LOWORD(lp), HIWORD(lp));
						}
						break;
					case WM_RBUTTONUP:
						if(psw2->pm){
							psw2->pm->b_rdown = false;
							psw2->pm->rbuttonup(LOWORD(lp), HIWORD(lp));
						}
						break;
				}
				return CallWindowProc((WNDPROC)psw2->OldStaticProc, hWnd, uMsg, wp, lp);
			}
		};
		struct _edit : public _gui{
			_wcolor wc;
			HBRUSH hBrush;
			_procedure *p_proc;
			FARPROC OldEditProc;
			int id;
			_edit(_procedure *p_proc, char *name, int x, int y, int w, int h, int id, int size, DWORD dw){
				this->h = CreateWindow(L"EDIT", (LPCWSTR)name, WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | ES_LEFT | WS_VSCROLL | dw,
					x, y, w, h, p_proc->hWnd, (HMENU)id, NULL, 0);
				setfont(this->h, size);
				this->p_proc = p_proc;
				this->id = id;
				setdata();
				//色
				wc.back.r = 255;wc.back.g = 255;wc.back.b = 255;
				wc.pen.r = 0;wc.pen.g = 0;wc.pen.b = 0;
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
			}
			_edit(_procedure *p_proc, char *name, int x, int y, int w, int h, int id, int size, bool b_subclass, DWORD dw){
				if(!b_subclass){
					this->h = CreateWindow(L"EDIT", (LPCWSTR)name, WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | 
						WS_HSCROLL |  WS_VSCROLL | dw,
						x, y, w, h, p_proc->hWnd, (HMENU)id, NULL, 0);
				}
				else{
					this->h = CreateWindow(L"EDIT", (LPCWSTR)name, WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | dw,
						x, y, w, h, p_proc->hWnd, (HMENU)id, NULL, 0);
				}
				setfont(this->h, size);
				this->p_proc = p_proc;
				this->id = id;
				setdata();
				//色
				wc.back.r = 255;wc.back.g = 255;wc.back.b = 255;
				wc.pen.r = 0;wc.pen.g = 0;wc.pen.b = 0;
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
				//サブクラス化
				if(b_subclass){
					OldEditProc = (FARPROC)GetWindowLong(this->h, GWL_WNDPROC);
					SetWindowLong(this->h, GWL_WNDPROC, (LONG)NewEditProc);
				}
			}
			void setcolor(byte back_r, byte back_g, byte back_b, byte pen_r, byte pen_g, byte pen_b){
				wc.back.r = back_r;wc.back.g = back_g;wc.back.b = back_b;
				wc.pen.r = pen_r;wc.pen.g = pen_g;wc.pen.b = pen_b;
				DeleteObject(hBrush);
				hBrush = CreateSolidBrush(RGB(back_r, back_g, back_b));
			}
			static LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp){
				_edit *pe = (_edit *)GetWindowLong(hWnd, GWL_USERDATA);
				if(uMsg == WM_KEYDOWN && wp == VK_RETURN){
					SendMessage(pe->p_proc->hWnd, WM_EDITRETURN, (LPARAM)pe, (LPARAM)hWnd);
					PostMessage(hWnd, WM_EDITRETURN2, pe->id, 0);
				}
				if(uMsg == WM_EDITRETURN2)
					SetWindowText(hWnd, L"");
				return CallWindowProc((WNDPROC)pe->OldEditProc, hWnd, uMsg, wp, lp);
			}
			void set(wchar_t *pc){SetWindowText(h, (LPCWSTR)pc);}
			void get(wchar_t *pc, unsigned length){GetWindowText(h, (LPWSTR)pc, length);}
			void get(char *pc, unsigned length){GetWindowTextA(h, pc, length);}
			int getint(){
				wchar_t c[256];
				get(c, 255);
				c[255] = 0;
				return _wtoi(c);
			}
			double getdouble(){
				wchar_t c[256];
				get(c, 255);
				c[255] = 0;
				return _wtof(c);
			}
			void setright(){
				LONG l = GetWindowLong(h, GWL_STYLE);
				SetWindowLong(h, GWL_STYLE, l | ES_RIGHT);
			}
			void setlimit(int i){SendMessage(h, EM_SETLIMITTEXT, (WPARAM)i, 0);}
			void scrolldown(){SendMessage(h, EM_SCROLL, SB_LINEDOWN, 0);}
		};
		struct _edit2 : public _staticwindow2{
			char *pc;
			int length;
			int lw, lh;
			int num_list;
			int i_pointer;
			int i_pointer_list;
			struct _mouse2 : public _mouse{
				_edit2 *pe2;
				_mouse2(_edit2 *pe2){
					this->pe2 = pe2;
				}
				void lbuttondown(int x, int y){
					pe2->inc();
					pe2->set_paint();
					pe2->invalidate();
				}
			};
			_edit2(_procedure *p_proc, int x, int y, int w, int h, int size) : _staticwindow2(p_proc, x, y, w, h, new _mouse2(this)){
				i_pointer = 0;
				i_pointer_list = 0;
				length = 0;
				num_list = 0;
				setfont(phm->h, size);
				lw = w;
				lh = h;
				pc = new char[0x100000];
			}
			~_edit2(){
				delete pc;
			}
			void selectfont(int size){
				setfont(phm->h, size);
			}
			void add(char *pc){
				int ai = 0;
				while(pc[ai]){
					this->pc[length] = pc[ai];
					inc_length();
					ai++;
				}
				this->pc[length] = '\n';
				inc_length();
				num_list++;
			}
			void inc(){
				while(pc[i_pointer]){
					i_pointer++;
					if(pc[i_pointer] == '\n'){
						i_pointer++;
						i_pointer_list++;
						break;
					}
				}
			}
			void sub(){
				while(i_pointer > 0){
					i_pointer--;
					if(pc[i_pointer - 1] == '\n'){
						i_pointer_list--;
						break;
					}
				}
			}
			void inc_length(){
				if(length != 0x100000 - 1)
					length++;
			}
			void set_paint(){
				phm->flush();
				//phm->rect(0, 0, lw, lh);
				RECT rc;
				GetClientRect(h, &rc);
				rc.top += 2;
				rc.bottom -= 2;
				rc.right -= 2;
				rc.left += 2;
				DrawText(phm->h, (LPCWSTR)(pc + i_pointer), -1, &rc, DT_EDITCONTROL);
			}
		};
		struct _list : public _gui{
			_wcolor wc;
			HBRUSH hBrush;
			FARPROC OldListProc;
			_procedure *p_proc;
			_list(_procedure *p_proc, char *name, int x, int y, int w, int h, int size){
				_list(p_proc, name, x, y, w, h, size, 0);
			}
			_list(_procedure *p_proc, char *name, int x, int y, int w, int h, int size, int id){
				this->h = CreateWindow(L"LISTBOX", (LPCWSTR)name, WS_VISIBLE | WS_CHILD | WS_VSCROLL | LBS_STANDARD,
					x, y, w, h, p_proc->hWnd, (HMENU)id, NULL, 0);
				setfont(this->h, size);
				setdata();
				this->p_proc = p_proc;
				//色
				wc.back.r = 255;wc.back.g = 255;wc.back.b = 255;
				wc.pen.r = 0;wc.pen.g = 0;wc.pen.b = 0;
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
				//サブクラス化
				OldListProc = (FARPROC)GetWindowLong(this->h, GWL_WNDPROC);
				SetWindowLong(this->h, GWL_WNDPROC, (LONG)NewListProc);
			}
			void setcolor(byte back_r, byte back_g, byte back_b, byte pen_r, byte pen_g, byte pen_b){
				wc.back.r = back_r;wc.back.g = back_g;wc.back.b = back_b;
				wc.pen.r = pen_r;wc.pen.g = pen_g;wc.pen.b = pen_b;
				DeleteObject(hBrush);
				hBrush = CreateSolidBrush(RGB(back_r, back_g, back_b));
			}
			void add(char *pc){ListBox_AddString(h, pc);}
			void add2(char *pc){
				add(pc);
				setcursel(getcount() - 1);
			}
			void add(wchar_t *pc){ListBox_AddString(h, pc);}
			void add2(wchar_t *pc){
				add(pc);
				setcursel(getcount() - 1);
			}
			void reset(){ListBox_ResetContent(h);}
			int getcursel(){return ListBox_GetCurSel(h);}
			void setcursel(int index){ListBox_SetCurSel(h, index);}
			int getcount(){return ListBox_GetCount(h);}
			static LRESULT CALLBACK NewListProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp){
				_list *pl = (_list *)GetWindowLong(hWnd, GWL_USERDATA);
				switch(uMsg){
					case WM_LBUTTONUP:
						SendMessage(pl->p_proc->hWnd, WM_LMLBUTTONUP, (WPARAM)pl, 0);
						break;
					case WM_LBUTTONDBLCLK:
						SendMessage(pl->p_proc->hWnd, WM_LMLBUTTONDBLCLK, (WPARAM)hWnd, 0);
						break;
				}
				return CallWindowProc((WNDPROC)pl->OldListProc, hWnd, uMsg, wp, lp);
			}
		};
		struct _listview : public _gui{
			_wcolor wc;
			void (*func)(HDC, RECT *, int, int);
			_listview(_procedure *p_proc, int x, int y, int w, int h, int size, void (*func)(HDC, RECT *, int, int)){
				if(func)
					this->h = CreateWindowEx(0, WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_OWNERDRAWFIXED,
						x, y, w, h, p_proc->hWnd, 0, 0, 0);
				else
					this->h = CreateWindowEx(0, WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | LVS_REPORT,
						x, y, w, h, p_proc->hWnd, 0, 0, 0);
				this->func = func;
				setdata();
				DWORD dwStyle;
				dwStyle = ListView_GetExtendedListViewStyle(this->h);
				dwStyle |= LVS_EX_FULLROWSELECT;// | LVS_EX_GRIDLINES;
				ListView_SetExtendedListViewStyle(this->h, dwStyle);
				wc.back.r = 255;wc.back.g = 255;wc.back.b = 255;
				wc.pen.r = 0;wc.pen.g = 0;wc.pen.b = 0;
				ListView_SetBkColor(this->h, RGB(255, 255, 255));
				setfont(this->h, size);
			}
			void setcolor(byte back_r, byte back_g, byte back_b, byte pen_r, byte pen_g, byte pen_b){
				wc.back.r = back_r;wc.back.g = back_g;wc.back.b = back_b;
				wc.pen.r = pen_r;wc.pen.g = pen_g;wc.pen.b = pen_b;
				ListView_SetBkColor(this->h, RGB(back_r, back_g, back_b));
			}
			void ins_cl(char *name, int x, int iSubItem){
				LV_COLUMN lvcol;
				lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
				lvcol.fmt = LVCFMT_LEFT;
				lvcol.cx = x;
				lvcol.pszText = (LPWSTR)name;
				lvcol.iSubItem = iSubItem;
				ListView_InsertColumn(this->h, iSubItem, &lvcol);
			}
			void ins_item(char *name, int iItem){
				LV_ITEM item;
				item.mask = LVIF_TEXT;
				item.pszText = (LPWSTR)name;
				item.iItem = iItem;
				item.iSubItem = 0;
				ListView_InsertItem(this->h, &item);
			}
			void set_item(char *name, int iSubItem, int iItem){
				LV_ITEM item;
				item.mask = LVIF_TEXT;
				item.pszText = (LPWSTR)name;
				item.iItem = iItem;
				item.iSubItem = iSubItem;
				ListView_SetItem(this->h, &item);
			}
			void resetcontent(HWND hLivi){
				while(1){
					int nItem = ListView_GetNextItem(this->h, -1, LVNI_ALL);
					if(nItem == -1)
						break;
					ListView_DeleteItem(this->h, nItem);
					ListView_Arrange(this->h, LVA_ALIGNLEFT);
				}
			}
			void DrawListItemText(HDC hdc,char *Text,RECT *rc,int fmt){
				DRAWTEXTPARAMS	DrawTextExParam;
				int		Justify;

				//隣り合う文字列がくっつかないように左右にマージンをつける。
				//ただし、右マージンは文字の配置を右寄せにしたときだけ付ける。
				DrawTextExParam.cbSize=sizeof(DrawTextExParam);
				DrawTextExParam.iLeftMargin=2;
				DrawTextExParam.iRightMargin=0;
				DrawTextExParam.iTabLength=0;
				DrawTextExParam.uiLengthDrawn=0;

				//サブアイテムの文字の配置を調べて、DrawTextEx用のスタイルに直す。
				if((fmt & LVCFMT_JUSTIFYMASK)==LVCFMT_LEFT)			Justify=DT_LEFT;
				else if((fmt & LVCFMT_JUSTIFYMASK)==LVCFMT_RIGHT){	Justify=DT_RIGHT;
																	DrawTextExParam.iRightMargin=6;}
				else												Justify=DT_CENTER;
				//文字列の表示　左右マージンをつけるのでDrawTextExを使っている。
				DrawTextEx(hdc,(LPWSTR)Text, -1,rc,
					Justify | DT_VCENTER | DT_SINGLELINE,
					&DrawTextExParam);
			}
			void drawlistview(LPDRAWITEMSTRUCT lpDraw){
				HWND		hList;				//リストビューのハンドル
				HDC			hdc;				//デバイスコンテキスト
				RECT		rc,rcAll;				
				HBRUSH		hBrush;			//背景描画用のブラシハンドル
				COLORREF	Color;
				char		Text[256];
				int			SubItem;
				LVCOLUMN	LvColumn;	//列項目取得用の構造体
				LVITEM		LvItem;			//アイテム情報取得用の構造体
				int			SubItemNum;
				HPEN		hPen;

				hList=lpDraw->hwndItem;
				hdc = lpDraw->hDC;
				hPen = CreatePen(PS_SOLID, 1, RGB(wc.pen.r, wc.pen.g, wc.pen.b));
				SelectObject(hdc, hPen);
				SetTextColor(hdc, RGB(wc.pen.r, wc.pen.g, wc.pen.b));
				SaveDC(hdc);
				//文字の背景の表示モードを非透過に設定する。
				SetBkMode(hdc,TRANSPARENT);
				//ODS_SELECTEDフラグがあるときには、強調表示する。
				if (lpDraw->itemState & ODS_SELECTED) {
					//背景色の取得
					hBrush=CreateSolidBrush (GetSysColor(COLOR_HIGHLIGHT));
					SetBkColor(hdc, GetSysColor(COLOR_HIGHLIGHT));
					SetTextColor(hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));
				}else{
					//Color=ListView_GetTextBkColor(hList);
					//ListView_SetTextBkColorが一度もされてないときには、ff000000という色になるので
					//その時には、システムの色を使う。これをしないと黒になってしまう。
					//if(Color & 0xff000000) Color=GetSysColor(COLOR_WINDOW);
					Color = RGB(wc.back.r, wc.back.g, wc.back.b);
					hBrush=CreateSolidBrush (Color);
				}

				ListView_GetItemRect(hList,lpDraw->itemID,&rcAll,LVIR_BOUNDS);
				ListView_GetItemRect(hList,lpDraw->itemID,&rc,LVIR_LABEL);
				FillRect(hdc,&rcAll,hBrush);
				ZeroMemory(&LvColumn,sizeof(LvColumn));
				LvColumn.mask=LVCF_FMT;
				ListView_GetColumn(hList,0,&LvColumn);
				//アイテムの情報を取得します。表示するイメージの番号もいるのでListView_GetItemを使う。
				ZeroMemory(&LvItem,sizeof(LvItem));
				LvItem.iItem=lpDraw->itemID;
				LvItem.iSubItem=0;
				LvItem.mask=LVIF_TEXT | LVIF_IMAGE;
				LvItem.pszText=(LPWSTR)Text;
				LvItem.cchTextMax=sizeof(Text);
				ListView_GetItem(hList,&LvItem);
				//文字列を表示する。ListView_GetItemRectを使うと表示する領域が分かる。
				ListView_GetItemRect(hList,lpDraw->itemID,&rc,LVIR_LABEL);
				DrawListItemText(hdc,Text,&rc,LvColumn.fmt);
				//サブアイテムの数を取得する
				SubItemNum=Header_GetItemCount(ListView_GetHeader(hList))-1;
				//サブアイテムも同様に表示する。
				for(SubItem=1;SubItem<=SubItemNum;SubItem++){
					//背景のクリア
					ListView_GetSubItemRect(hList,lpDraw->itemID,SubItem,LVIR_BOUNDS,&rc);
					FillRect(hdc,&rc,hBrush);
					//列項目のフォーマットを得ます。構造体はさっき設定したのでそのまま使う。
					ListView_GetColumn(hList,SubItem,&LvColumn);
					ListView_GetSubItemRect(hList,lpDraw->itemID,SubItem,LVIR_LABEL,&rc);
					//アイテムの情報を取得します。文字列だけなので今度はListView_GetItemTextを使います。
					ListView_GetItemText(hList,lpDraw->itemID,SubItem,(LPWSTR)Text,sizeof(Text));
					DrawListItemText(hdc,Text,&rc,LvColumn.fmt);
					func(hdc, &rc, LvItem.iItem, SubItem);
				}
				if (lpDraw->itemState & ODS_FOCUS){
					DrawFocusRect(hdc,&rcAll);
				}
				DeleteObject(hPen);
				DeleteObject(hBrush);
				RestoreDC(hdc,-1);
			}
		};
		struct _tab : public _gui{
			_tab(_procedure *p_proc, int x, int y, int w, int h){
				this->h= CreateWindowEx(0, WC_TABCONTROL, 0, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
					x, y, w, h, p_proc->hWnd, 0, NULL, 0);
				setfont(this->h, 12);
				setdata();
			}
			void ins(LPWSTR name, int id){
				TC_ITEM tcItem;
				tcItem.mask = TCIF_TEXT;
				tcItem.pszText = name;
				SendMessage(this->h, TCM_INSERTITEM, (WPARAM)id, (LPARAM)&tcItem);
			}
			int get(){return TabCtrl_GetCurSel(this->h);}
			void set(int i){TabCtrl_SetCurSel(this->h, i);}
		};
		_procedure(){
			b_ldown = false;
			b_rdown = false;
			hBrush_edit = CreateSolidBrush(RGB(0, 0, 255));
		}
		virtual void operator()(_arg &arg){};
		virtual void init(){}
		virtual void mousemove(int x, int y){}
		virtual void lbuttondown(int x, int y){}
		virtual void rbuttondown(int x, int y){}
		virtual void lbuttonup(int x, int y){}
		virtual void rbuttonup(int x, int y){}
		void setpos(int x, int y, int w, int h){SetWindowPos(hWnd, NULL, x, y, w + 6, h + 32, NULL);}
		void ctasktray(int id, int message){
			NOTIFYICONDATA ni;
			memset(&ni, 0, sizeof(NOTIFYICONDATA));
			ni.cbSize = sizeof(NOTIFYICONDATA);
			ni.hWnd = hWnd;
			ni.uID = (UINT)id;
			ni.hIcon = (HICON)LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
			ni.uCallbackMessage = (UINT)message;
			ni.uFlags = NIF_ICON | NIF_MESSAGE;
			Shell_NotifyIcon(NIM_ADD, &ni);
		}
		void show(){ShowWindow(hWnd, SW_SHOWNORMAL);}
		void hide(){ShowWindow(hWnd, SW_HIDE);}
		void dtasktray(int id){
			NOTIFYICONDATA ni;
			memset(&ni, 0, sizeof(NOTIFYICONDATA));
			ni.cbSize = sizeof(NOTIFYICONDATA);
			ni.hWnd = hWnd;
			ni.uID = (UINT)id;
			ni.hIcon = (HICON)LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
			ni.uFlags = NIF_ICON | NIF_MESSAGE;
			Shell_NotifyIcon(NIM_DELETE, &ni);
		}
		void cmenu(HMENU hMenu, int x, int y){TrackPopupMenu(hMenu, TPM_LEFTALIGN, x, y, 0, hWnd, NULL);}
		void cmenu(char *pc){
			HMENU hMenu = LoadMenu(NULL, (LPCWSTR)pc);
			HMENU hSubMenu = GetSubMenu(hMenu, 0);
			POINT pt;
			GetCursorPos(&pt);
			cmenu(hSubMenu, pt.x, pt.y);
			DestroyMenu(hMenu);
		}
		_button *button(wchar_t *name, int x, int y, int w, int h, int id){return new _button(this, name, x, y, w, h, id);}
		_button *button(wchar_t *name, int x, int y, int w, int h, int id, HDC hdc_normal, HDC hdc_selected){return new _button(this, name, x, y, w, h, id, hdc_normal, hdc_selected);}
		_button *colorbutton(wchar_t *name, int x, int y, int w, int h, int id){
			hdcmem *phm_normal, *phm_selected;
			phm_normal = new hdcmem(w, h);
			phm_selected = new hdcmem(w, h);
			unsigned len = wslen(name);
			HPEN hPen;
			HFONT hFont;

			phm_normal->gradrect(0, 0, w - 1, h - 1, 255, 255, 255, 127, 127, 255);
			hPen = phm_normal->selectpen(0, 0, 255);
			phm_normal->rect(0, 0, w - 1, h - 1);
			DeleteObject(hPen);

			phm_selected->gradrect(1, 1, w, h, 255, 255, 255, 127, 127, 255);
			hPen = phm_selected->selectpen(0, 0, 255);
			phm_selected->rect(1, 1, w, h);
			DeleteObject(hPen);
			
			hFont = phm_normal->selectfont(16);
			phm_normal->settextcolor(127, 127, 127);
			phm_normal->string(w / 2 - 4 * len + 1, 2, (LPCWSTR)name);
			phm_normal->settextcolor(0, 0, 127);
			phm_normal->string(w / 2 - 4 * len, 1, (LPCWSTR)name);
			DeleteObject(hFont);

			hFont = phm_selected->selectfont(16);
			phm_selected->settextcolor(127, 127, 127);
			phm_selected->string(w / 2 - 4 * len + 2, 3, (LPCWSTR)name);
			phm_selected->settextcolor(0, 0, 127);
			phm_selected->string(w / 2 - 4 * len + 1, 2, (LPCWSTR)name);
			DeleteObject(hFont);

			return button(name, x, y, w, h, id, phm_normal->h, phm_selected->h);
		}
		int wslen(wchar_t *pc){
			for(int i = 0; i < 256; i++){
				if(pc[i] == 0)
					return i;
			}
			return -1;
		}
		_radiobutton *radiobutton(int x, int y, int w, int h, int id){return new _radiobutton(this, x, y, w, h, id);}
		_radiobutton2 *radiobutton2(wchar_t *pc, int x, int y, int w, int h, int id, bool b_start){return new _radiobutton2(this, pc, x, y, w, h, id, b_start);}
		_edit *edit(int x, int y, int w, int h, int id){return new _edit(this, "", x, y, w, h, id, 12, true, NULL);}
		_edit *edit(int x, int y, int w, int h, int id, bool b_true){return new _edit(this, "", x, y, w, h, id, 12, NULL);}
		_edit *edit(int x, int y, int w, int h, int id, DWORD dw){return new _edit(this, "", x, y, w, h, id, 12, true, dw);}
		_edit *edit(int x, int y, int w, int h, int id, DWORD dw, bool b){return new _edit(this, "", x, y, w, h, id, 12, b, dw);}
		_edit2 *edit2(int x, int y, int w, int h){return new _edit2(this, x, y, w, h, 10);}
		_edit2 *edit2(int x, int y, int w, int h, int size, char *pc){
			_edit2 *pe2 = new _edit2(this, x, y, w, h, size);
			pe2->add(pc);
			pe2->set_paint();
			return pe2;
		}
		_edit *mledit(int x, int y, int w, int h, int id){return new _edit(this, "", x, y, w, h, id, 12, false, NULL);}
		_list *list(int x, int y, int w, int h){return new _list(this, "", x, y, w, h, 12);}
		_list *list(int x, int y, int w, int h, int id){return new _list(this, "", x, y, w, h, 12, id);}
		_listview *listview(int x, int y, int w, int h){return new _listview(this, x, y, w, h, 12, NULL);}
		_listview *listview(int x, int y, int w, int h, void (*func)(HDC, RECT *, int, int)){return new _listview(this, x, y, w, h, 12, func);}
		_tab *tab(int x, int y, int w, int h){return new _tab(this, x, y, w, h);}
		_staticwindow *staticwindow(char *name, int x, int y, int w, int h){return new _staticwindow(this, name, x, y, w, h);}
		_staticwindow *staticwindow(int x, int y, int w, int h){return new _staticwindow(this, x, y, w, h);}
		_staticwindow *staticwindow(int x, int y, int w, int h, _staticwindow::_mouse *pm){return new _staticwindow(this, x, y, w, h, pm);}
		_staticwindow2 *staticwindow2(int x, int y, int w, int h){return new _staticwindow2(this, x, y, w, h);}
		_staticwindow2 *staticwindow2(int x, int y, int w, int h, _staticwindow2::_mouse *pm){return new _staticwindow2(this, x, y, w, h, pm);}
		static void setfont(HWND hWnd, int size){
			HFONT hFont = CreateFont(size, size / 2, 0, 0, 0,
				FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"ＭＳ ゴシック");
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, 1);
		}
		static void setfont(HDC hdc, int size){
			HFONT hFont = CreateFont(size, size / 2, 0, 0, 0,
				FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"ＭＳ ゴシック");
			SelectFont(hdc, hFont);
		}
	}*p_proc;
	static unsigned count;
	static bool b_initcc;
	bool b_show;
	bool b_drag;
	window(_procedure *p_proc){
		b_show = true;
		this->pv = NULL;
		sub_constructor(p_proc, WIDTH_WINDOW, HEIGHT_WINDOW, WIDTH_WINDOW, HEIGHT_WINDOW, NULL);
	}
	window(_procedure *p_proc, int x, int y){
		b_show = true;
		this->pv = NULL;
		sub_constructor(p_proc, x, y,
			(GetSystemMetrics(SM_CXSCREEN) - x) / 2, (GetSystemMetrics(SM_CYSCREEN) - y) / 2, NULL);
	}
	window(_procedure *p_proc, int x, int y, void *pv){
		b_show = true;
		this->pv = pv;
		sub_constructor(p_proc, x, y,
			(GetSystemMetrics(SM_CXSCREEN) - x) / 2, (GetSystemMetrics(SM_CYSCREEN) - y) / 2, NULL);
	}
	window(_procedure *p_proc, int x, int y, int x2, int y2){
		b_show = true;
		this->pv = NULL;
		sub_constructor(p_proc, x, y, x2, y2, NULL);
	}
	window(_procedure *p_proc, int x, int y, int x2, int y2, void *pv){
		b_show = true;
		this->pv = pv;
		sub_constructor(p_proc, x, y, x2, y2, NULL);
	}
	window(_procedure *p_proc, int x, int y, int x2, int y2, void *pv, char *pc_menu){
		b_show = true;
		this->pv = pv;
		sub_constructor(p_proc, x, y, x2, y2, pc_menu);
	}
	window(_procedure *p_proc, int x, int y, int x2, int y2, void *pv, char *pc_menu, DWORD dw){
		b_show = true;
		this->pv = pv;
		sub_constructor(p_proc, x, y, x2, y2, pc_menu, dw);
	}
	window(_procedure *p_proc, int x, int y, void *pv, bool b_show){
		this->pv = pv;
		this->b_show = b_show;
		sub_constructor(p_proc, x, y,
			(GetSystemMetrics(SM_CXSCREEN) - x) / 2, (GetSystemMetrics(SM_CYSCREEN) - y) / 2, NULL);
	}
	window(_procedure *p_proc, bool b_show){
		this->b_show = b_show;
		this->pv = NULL;
		sub_constructor(p_proc, 0, 0, 0, 0, NULL);
	}
	void sub_constructor(_procedure *p_proc, int x, int y, int x2, int y2, char *pc_menu){
		sub_constructor(p_proc, x, y, x2, y2, pc_menu, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
	}
	void sub_constructor(_procedure *p_proc, int x, int y, int x2, int y2, char *pc_menu, DWORD dw){
		MSG msg;
		WNDCLASS wc;
		HWND hWnd;
		HINSTANCE hInst;
		this->p_proc = p_proc;
		char cname[256];
		b_drag = false;
		wsprintf((LPWSTR)cname, L"MyWindow%d", count);
		count++;
		p_proc->phm = new hdcmem(x, y);
		p_proc->pw = this;
		if(!b_initcc){
			InitCommonControls();
			b_initcc = true;
		}

		hInst = (HINSTANCE)GetWindowLong(NULL, GWL_HINSTANCE);
		p_proc->hInst = hInst;
		ZeroMemory(&wc, sizeof(wc));
		wc.lpfnWndProc = WndProc;
		wc.hInstance = hInst;
		wc.hIcon = LoadIcon(0, IDI_APPLICATION);
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
		wc.lpszClassName = (LPCWSTR)cname;
		wc.lpszMenuName = (LPCWSTR)pc_menu;
		if(RegisterClass(&wc) == 0){
			MessageBox(NULL, L"ウィンドウクラスの登録に失敗しました。", L"Error", MB_OK);
			return;
		}
		regist_static2();
		hWnd = CreateWindow((LPCWSTR)cname, L"Window", dw,
			x2, y2, 
			x + 6, y + 32, 0, 0, hInst, NULL);
		if(hWnd == 0){
			MessageBox(NULL, L"ウィンドウの作成に失敗しました。", L"Error", MB_OK);
			return;
		}
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)this);
		p_proc->hWnd = hWnd;
		SendMessage(hWnd, WM_INIT, 0, 0);

		if(b_show)
			ShowWindow(hWnd, SW_SHOWNORMAL);
		else
			ShowWindow(hWnd, SW_HIDE);
		UpdateWindow(hWnd);

		while(GetMessage(&msg, 0, 0, 0)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	void regist_static2(){
		//MSG msg;
		WNDCLASS wc;
		//HWND hWnd;
		HINSTANCE hInst;

		hInst = (HINSTANCE)GetWindowLong(NULL, GWL_HINSTANCE);
		ZeroMemory(&wc, sizeof(wc));
		wc.lpfnWndProc = WndProc_static2;
		wc.hInstance = hInst;
		wc.hIcon = LoadIcon(0, IDI_APPLICATION);
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
		wc.lpszClassName = L"static2";
		if(RegisterClass(&wc) == 0){
			char c[256];
			if(GetLastError() != 1410){
				wsprintf((LPWSTR)c, L"ウィンドウクラスの登録に失敗しました。(static2, %d)", GetLastError());
				MessageBox(NULL, (LPCWSTR)c, L"Error", MB_OK);
			}
			return;
		}
	}
	~window(){UnregisterClass((LPCWSTR)cname, (HINSTANCE)GetWindowLong(NULL, GWL_HINSTANCE));}
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp){
		_arg arg = {hWnd, uMsg, wp, lp};
		window *pw = (window *)GetWindowLong(hWnd, GWL_USERDATA);
		if(pw != NULL){
			window::_procedure *p_proc = pw->p_proc;
			p_proc->b_return = false;
			(*p_proc)(arg);
			if(p_proc->b_return)
				return p_proc->lr_return;
			switch(uMsg){
				case WM_INIT:
					p_proc->init();
					break;
				case WM_MOUSEMOVE:
					p_proc->mousemove(LOWORD(lp), HIWORD(lp));
					break;
				case WM_LBUTTONDOWN:
					p_proc->b_ldown = true;
					p_proc->lbuttondown(LOWORD(lp), HIWORD(lp));
					break;
				case WM_RBUTTONDOWN:
					p_proc->b_rdown = true;
					p_proc->rbuttondown(LOWORD(lp), HIWORD(lp));
					break;
				case WM_LBUTTONUP:
					p_proc->b_ldown = false;
					p_proc->lbuttonup(LOWORD(lp), HIWORD(lp));
					break;
				case WM_RBUTTONUP:
					p_proc->b_rdown = false;
					p_proc->rbuttonup(LOWORD(lp), HIWORD(lp));
					break;
				case WM_DESTROY:
					PostQuitMessage(0);
					return 0;
				case WM_PAINT:
					{
						PAINTSTRUCT ps;
						HDC hdc = BeginPaint(hWnd, &ps);
						p_proc->phm->copyto(hdc);
						EndPaint(hWnd, &ps);
					}
					break;
				case WM_CTLCOLOREDIT:
					{
						HWND hWnd_edit = (HWND)lp;
						_procedure::_edit *pe = (_procedure::_edit *)GetWindowLong(hWnd_edit, GWL_USERDATA);
						if(pe){
							SetBkMode((HDC)wp, OPAQUE);
							SetTextColor((HDC)wp, RGB(pe->wc.pen.r, pe->wc.pen.g, pe->wc.pen.b));
							SetBkColor((HDC)wp, RGB(pe->wc.back.r, pe->wc.back.g, pe->wc.back.b));
						}
						return (LRESULT)pe->hBrush;
					}
					break;
				case WM_CTLCOLORLISTBOX:
					{
						HWND hWnd_list = (HWND)lp;
						_procedure::_list *pl = (_procedure::_list *)GetWindowLong(hWnd_list, GWL_USERDATA);
						if(pl){
							SetBkMode((HDC)wp, OPAQUE);
							SetTextColor((HDC)wp, RGB(pl->wc.pen.r, pl->wc.pen.g, pl->wc.pen.b));
							SetBkColor((HDC)wp, RGB(pl->wc.back.r, pl->wc.back.g, pl->wc.back.b));
						}
						return (LRESULT)pl->hBrush;
					}
					break;
				case WM_DRAWITEM:
					{
						LPDRAWITEMSTRUCT lpds = (LPDRAWITEMSTRUCT)lp;
						switch(lpds->CtlType){
						case ODT_BUTTON:
							{
								HDC hdc = lpds->hDC;
								_procedure::_button *pb = (_procedure::_button *)GetWindowLong(lpds->hwndItem, GWL_USERDATA);
								HDC hdc_normal = pb->hdc_normal;
								HDC hdc_selected = pb->hdc_selected;
								if(lpds->CtlType == ODT_BUTTON){
									if(lpds->itemState & ODS_SELECTED)
										BitBlt(hdc, 0, 0, lpds->rcItem.right, lpds->rcItem.bottom, hdc_selected, 0, 0, SRCCOPY);
									else
										BitBlt(hdc, 0, 0, lpds->rcItem.right, lpds->rcItem.bottom, hdc_normal, 0, 0, SRCCOPY);
								}
							}
							break;
						case ODT_LISTVIEW:
							{
								_procedure::_listview *plv = (_procedure::_listview *)GetWindowLong(lpds->hwndItem, GWL_USERDATA);
								plv->drawlistview((LPDRAWITEMSTRUCT)lp);
							}
							break;
						}
					}
				case WM_NOTIFY:
					{
						LPNMHDR lpnmhdr;
						LPNMLISTVIEW lplv;
						LPNMLVCUSTOMDRAW lplvcd;
						lpnmhdr = (LPNMHDR)lp;
						lplv = (LPNMLISTVIEW)lp;
						HWND hWnd_listview = lpnmhdr->hwndFrom;
						_procedure::_listview *plv = (_procedure::_listview *)GetWindowLong(hWnd_listview, GWL_USERDATA);
						switch(lplv->hdr.code){
						case NM_CUSTOMDRAW:
							lplvcd = (LPNMLVCUSTOMDRAW)lp;
							if(lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
								return CDRF_NOTIFYITEMDRAW;
							if(lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT){
								lplvcd->clrTextBk = RGB(plv->wc.back.r, plv->wc.back.g, plv->wc.back.b);
								lplvcd->clrText = RGB(plv->wc.pen.r, plv->wc.pen.g, plv->wc.pen.b);
								return CDRF_NEWFONT;
							}
							break;
						case TCN_SELCHANGE:
							{
								HWND hWnd_tab = lpnmhdr->hwndFrom;
								_procedure::_tab *pt = (_procedure::_tab *)GetWindowLong(hWnd_tab, GWL_USERDATA);
								PostMessage(hWnd, WM_TBCHANGE, (WPARAM)pt, (LPARAM)hWnd_tab);
							 }
							break;
						}
						if(lpnmhdr->code == NM_DBLCLK){
							int index = ListView_GetNextItem(lpnmhdr->hwndFrom, -1, LVNI_SELECTED);
							if(index == -1)
								break;;
							PostMessage(hWnd, WM_LVDBLCLK, (WPARAM)plv, index);
						}
					}
				break;
			case WM_NCHITTEST:
				{
					if(pw->b_drag){
						LRESULT lResult = DefWindowProc(hWnd, WM_NCHITTEST, wp, lp);
						if((HTCLIENT == lResult)){
							return HTCAPTION;
						}
						return lResult;
					}
					break;
				}
			}
		}
		return DefWindowProc(hWnd, uMsg, wp, lp);
	}
	static LRESULT CALLBACK WndProc_static2(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp){
		return DefWindowProc(hWnd, uMsg, wp, lp);
	}
};
unsigned window::count = 0;
bool window::b_initcc = false;
typedef window::_procedure::_button _button;
typedef window::_procedure::_radiobutton _radiobutton;
typedef window::_procedure::_radiobutton2 _radiobutton2;
typedef window::_procedure::_staticwindow _staticwindow;
typedef window::_procedure::_staticwindow2 _staticwindow2;
typedef window::_procedure::_edit _edit;
typedef window::_procedure::_edit2 _edit2;
typedef window::_procedure::_list _list;
typedef window::_procedure::_listview _listview;
typedef window::_procedure::_tab _tab;
#endif