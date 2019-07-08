#ifndef MINE_SAFEQUEUE
#define MINE_SAFEQUEUE

#include "INCLUDE/queue2.h"

template <typename T>
class safequeue : protected queue2<T>{
	CRITICAL_SECTION cs;
	HANDLE h;
public:
	safequeue(){
		InitializeCriticalSection(&cs);
		h = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	~safequeue(){
		DeleteCriticalSection(&cs);
		CloseHandle(h);
	}
	T *push(T &t){
		T *pt;
		bool b = iszero();
		EnterCriticalSection(&cs);
		pt = queue2<T>::push(t);
		LeaveCriticalSection(&cs);
		if(b)
			SetEvent(h);
		return pt;
	}
	void copy(T &t){
		EnterCriticalSection(&cs);
		t = peek();
		LeaveCriticalSection(&cs);
	}
	void pop(){
		EnterCriticalSection(&cs);
		queue2<T>::pop();
		LeaveCriticalSection(&cs);
		if(iszero())
			ResetEvent(h);
	}
	void wait(){WaitForSingleObject(h, INFINITE);}
	void wait(unsigned dwMilliseconds){WaitForSingleObject(h, dwMilliseconds);}
	void enter(){EnterCriticalSection(&cs);}
	void leave(){LeaveCriticalSection(&cs);}
	T &peek(){return *queue2<T>::end();}
	T *end(){return queue2<T>::end();}
	T *front(T *pt){return queue2<T>::front(pt);}
	bool iszero(){
		enter();
		bool b = (queue2<T>::getnumber() == 0);
		leave();
		return b;
	}
	int getnumber(){return queue2<T>::getnumber();}
};
#endif