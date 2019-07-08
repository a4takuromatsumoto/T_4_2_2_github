#pragma once
#include <process.h>
#include "INCLUDE/safequeue.h"
#include "INCLUDE/window2.h"

#define M2_MAX_NAME 32
#define M2_STATE_START 0
#define M2_STATE_HEADER 1
#define M2_STATE_BODY 2

class ws{
public:
	ws(){}
	static void wscpy(wchar_t *pc, wchar_t *pc2, int i_len){
		for(int i = 0; i < i_len; i++){
			pc[i] = pc2[i];
			if(pc[i] == 0)
				break;
		}
		pc[i_len] = 0;
	}
	static int wslen(wchar_t *pc){
		int i = 0;
		while(pc[i]){
			i++;
		}
		return i;
	}
	static bool wscmp(wchar_t *pc, wchar_t *pc2, int i_len){
		for(int i = 0; i < i_len; i++){
			if(pc[i] != pc2[i])
				return false;
			if(pc[i] == 0)
				return false;
		}
		return true;
	}
	static bool wscmp2(wchar_t *pc, wchar_t *pc2, int i_max){
		for(int i = 0; i < i_max; i++){
			if(pc[i] != pc2[i])
				return false;
			if(pc[i] == 0)
				return true;
		}
		return true;
	}
};
class multithread2{
public:
	unsigned num_sig_now;
	struct _data{
		struct _body{
			char *pc;
			unsigned length;
		}body;
		unsigned num_sig_from;
		wchar_t name_to[M2_MAX_NAME];
		//外部用
		bool b_candel;
		bool b_connect;
		bool b_shutdown;
		DWORD dw_addr;
	};
	struct _data_sl{
		_data *pd;
		bool b_connect;
		int state;
	};
	int num_network;

	struct _thread{
		bool b_window;
		multithread2 *pmt;
		unsigned num_sig;
		wchar_t name[M2_MAX_NAME];
		safequeue<_data> sq;
		virtual void operator()(){}
		void send(wchar_t *name_to, void *pc, unsigned length, bool b_candel){
			_thread *ptr = pmt->search(name_to);
			if(!ptr){
				if(b_candel)
					delete pc;
				return;
			}
			_data d;
			d.body.pc = (char *)pc;
			d.body.length = length;

			d.num_sig_from = num_sig;
			ZeroMemory(d.name_to, M2_MAX_NAME);

			d.b_candel = b_candel;
			d.b_connect = false;
			d.b_shutdown = false;
			d.dw_addr = NULL;

			ptr->sq.push(d);
			if(ptr->b_window)
				PostMessage(((windowthread *)ptr)->p_proc->hWnd, WM_QUEUEMESSAGE, 0, 0);
		}
		static void send_static(multithread2 *pmt, wchar_t *name_to, void *pc, unsigned length, bool b_candel){
			_thread *ptr = pmt->search(name_to);
			if(!ptr){
				if(b_candel)
					delete pc;
				return;
			}
			_data d;
			d.body.pc = (char *)pc;
			d.body.length = length;

			d.num_sig_from = -1;
			ZeroMemory(d.name_to, M2_MAX_NAME);

			d.b_candel = b_candel;
			d.b_connect = false;
			d.b_shutdown = false;
			d.dw_addr = NULL;

			ptr->sq.push(d);
			if(ptr->b_window)
				PostMessage(((windowthread *)ptr)->p_proc->hWnd, WM_QUEUEMESSAGE, 0, 0);
		}
		template<typename T>
			void send(char *name_to, T &t){
				T *pt = new T;
				*pt = t;
				send(name_to, pt, sizeof(t), true);
			}
		void send(DWORD dw_addr, wchar_t *name_to, void *pc, unsigned length, bool b_candel){
			_data *pd = new _data;
			pd->body.pc = (char *)pc;
			pd->body.length = length;

			pd->num_sig_from = 0;
			ZeroMemory(pd->name_to, M2_MAX_NAME);
			ws::wscpy(pd->name_to, name_to, M2_MAX_NAME);

			pd->b_candel = b_candel;
			pd->b_connect = false;
			pd->b_shutdown = false;
			pd->dw_addr = dw_addr;

			send(L"network", pd, sizeof(_data), true);
		}
		void sub_send(_data &d){
			//外部から
			d.name_to[M2_MAX_NAME - 1] = 0;
			_thread *ptr = pmt->search(d.name_to);
			if(!ptr || ptr->num_sig == pmt->num_network){
				delete d.body.pc;
				return;
			}
			ptr->sq.push(d);
			if(ptr->b_window)
				PostMessage(((windowthread *)ptr)->p_proc->hWnd, WM_QUEUEMESSAGE, 0, 0);
		}
		//キュー
		void wait(){sq.wait();}
		void wait(DWORD dwMilliseconds){sq.wait(dwMilliseconds);}
		_data *peek(){return &sq.peek();}
		void pop(){
			if(sq.peek().b_candel)
				delete sq.peek().body.pc;
			sq.pop();
		}
		void pop_nodelete(){sq.pop();}
		bool iszero(){return sq.iszero();}
		//プロセスの終了
		void end(){SetEvent(pmt->h_wait);}
	};
	struct windowthread : public _thread{
		window::_procedure *p_proc;
		int x, y, x2, y2;
		char *pc_menu;
		bool b_hide;
		bool b_dw;
		DWORD dw;
		windowthread(window::_procedure *p_proc, int x, int y){
			this->p_proc = p_proc;
			this->x = x;
			this->y = y;
			this->x2 = (GetSystemMetrics(SM_CXSCREEN) - x) / 2;
			this->y2 = (GetSystemMetrics(SM_CYSCREEN) - y) / 2;
			this->pc_menu = NULL;
			b_hide = false;
			b_dw = false;
		}
		windowthread(window::_procedure *p_proc, bool b_hide){
			this->p_proc = p_proc;
			this->x = 0;
			this->y = 0;
			this->x2 = 0;
			this->y2 = 0;
			this->pc_menu = NULL;
			this->b_hide = b_hide;
			b_dw = false;
		}
		windowthread(window::_procedure *p_proc, int x, int y, char *pc_menu){
			this->p_proc = p_proc;
			this->x = x;
			this->y = y;
			this->x2 = (GetSystemMetrics(SM_CXSCREEN) - x) / 2;
			this->y2 = (GetSystemMetrics(SM_CYSCREEN) - y) / 2;
			this->pc_menu = pc_menu;
			b_hide = false;
			b_dw = false;
		}
		windowthread(window::_procedure *p_proc, int x, int y, char *pc_menu, DWORD dw){
			this->p_proc = p_proc;
			this->x = x;
			this->y = y;
			this->x2 = (GetSystemMetrics(SM_CXSCREEN) - x) / 2;
			this->y2 = (GetSystemMetrics(SM_CYSCREEN) - y) / 2;
			this->pc_menu = pc_menu;
			b_hide = false;
			b_dw = true;
			this->dw = dw;
		}
		windowthread(window::_procedure *p_proc, int x, int y, int x2, int y2){
			this->p_proc = p_proc;
			this->x = x;
			this->y = y;
			this->x2 = x2;
			this->y2 = y2;
			this->pc_menu = NULL;
			b_hide = false;
			b_dw = false;
		}
		void operator()(){
			b_window = true;
			if(b_dw){
				if(b_hide){
					window(p_proc, x, y, x2, y2, this, pc_menu, dw);
				}
				else
					window(p_proc, x, y, x2, y2, this, false, dw);
			}
			else{
				if(b_hide){
					window(p_proc, x, y, x2, y2, this, false);
				}
				else
					window(p_proc, x, y, x2, y2, this, pc_menu);
			}
		}
	};
	list2<_thread *> l_tr;
	HANDLE h_wait;
	multithread2(){
		//psl = NULL;
		num_sig_now = 0;
		num_network = -1;
		h_wait = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	~multithread2(){CloseHandle(h_wait);}
	unsigned begin(_thread *ptr, wchar_t *name){
		if(!search(name)){
			ZeroMemory(ptr->name, M2_MAX_NAME);
			ws::wscpy(ptr->name, name, M2_MAX_NAME - 1);
			ptr->name[M2_MAX_NAME - 1] = 0;
			ptr->num_sig = num_sig_now;
			num_sig_now++;
			ptr->b_window = false;
			ptr->pmt = this;
			l_tr.ins(ptr);
			_beginthread(startofthread, 0, ptr);
			return ptr->num_sig;
		}
		return -1;
	}
	unsigned begin(window::_procedure *p_proc, wchar_t *name, int x, int y){
		_thread *ptr = new windowthread(p_proc, x, y);
		return begin(ptr, name);
	}
	unsigned begin(window::_procedure *p_proc, wchar_t *name, int x, int y, char *pc_menu){
		_thread *ptr = new windowthread(p_proc, x, y, pc_menu);
		return begin(ptr, name);
	}
	unsigned begin_nowindow(window::_procedure *p_proc, wchar_t *name){
		_thread *ptr = new windowthread(p_proc, true);
		return begin(ptr, name);
	}
	unsigned begin(window::_procedure *p_proc, wchar_t *name, int x, int y, int w, int h){
		_thread *ptr = new windowthread(p_proc, w, h, x, y);
		return begin(ptr, name);
	}
	unsigned begin(window::_procedure *p_proc, wchar_t *name, int w, int h, char *pc_menu, DWORD dw){
		_thread *ptr = new windowthread(p_proc, w, h, pc_menu, dw);
		return begin(ptr, name);
	}
	_thread *search(wchar_t *name){
		for(_thread **pptr = l_tr.start(); pptr != 0; pptr = l_tr.next(pptr)){
			if(ws::wscmp2((**pptr).name, name, 256))
				return *pptr;
		}
		return NULL;
	}
	_thread **search2(wchar_t *name){
		for(_thread **pptr = l_tr.start(); pptr != 0; pptr = l_tr.next(pptr)){
			if(ws::wscmp2((**pptr).name, name, 256))
				return pptr;
		}
		return NULL;
	}
	void del(wchar_t *name){
		_thread **pptr = search2(name);
		if(pptr){
			l_tr.del(pptr);
		}
	}
	static inline _thread *getpointer(window::_procedure *p_proc){return (multithread2::_thread *)p_proc->pw->pv;}
	void printlist(){
		for(_thread **pptr = l_tr.start(); pptr != 0; pptr = l_tr.next(pptr))
			wprintf(L"%d:%s\n", (**pptr).num_sig, (**pptr).name);
	}
	void wait(){WaitForSingleObject(h_wait, INFINITE);}
private:
	static void startofthread(void *pv){(*(_thread *)pv)();}
};
typedef multithread2 multithread;