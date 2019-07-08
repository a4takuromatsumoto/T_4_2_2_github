#pragma once
#include "INCLUDE/window2.h"
#include "INCLUDE/list2.h"
#include "conversation.h"


class socketlist{
	CRITICAL_SECTION cs;
public:
	struct _data_sk{
		SOCKET sk;
		wchar_t wc_ip[256];
		int i_port;
		conversation *pcv;
		int i_state;
	};
	list2<_data_sk> l_d_sk;
	bool b_cs;
	socketlist(){
		InitializeCriticalSection(&cs);
		b_cs = false;
	}
	~socketlist(){
		DeleteCriticalSection(&cs);
	}
	void enter(){EnterCriticalSection(&cs);b_cs = true;}
	void leave(){LeaveCriticalSection(&cs);b_cs = false;}
	_data_sk *ins(SOCKET sk, wchar_t *pwc_ip, int i_port, int i_state){
		enter();
		conversation *pcv = new conversation();
		_data_sk d_sk;
		d_sk.sk = sk;
		ws::wscpy(d_sk.wc_ip, pwc_ip, 256);
		d_sk.i_port = i_port;
		d_sk.pcv = pcv;
		d_sk.i_state = i_state;
		_data_sk *pd_sk = l_d_sk.ins(d_sk);
		leave();
		return pd_sk;
	}
	_data_sk *search_d(SOCKET s){
		enter();
		for(_data_sk *pd_sk = l_d_sk.start(); pd_sk != NULL; pd_sk = l_d_sk.next(pd_sk)){
			if(pd_sk->sk == s){
				leave();
				return pd_sk;
			}
		}
		leave();
		return NULL;
	}
	void del_d_sk(SOCKET s){
		_data_sk *pd_sk = search_d(s);
		l_d_sk.del(pd_sk);
	}
	_data_sk *search_ar(int i){
		_data_sk *pd_sk = l_d_sk.start();
		enter();
		for(int j = 0; j < i; j++){
			pd_sk = l_d_sk.next(pd_sk);
		}
		leave();
		return pd_sk;
	}
	void set(_list *pl){
		pl->reset();
		wchar_t c[1024];
		enter();
		for(_data_sk *pd_sk = l_d_sk.start(); pd_sk != NULL; pd_sk = l_d_sk.next(pd_sk)){
			switch(pd_sk->i_state){
			case 1:
				wsprintf(c, L"(%s:%d:%d)(s->c)", pd_sk->wc_ip, pd_sk->i_port, pd_sk->sk);
				break;
			case 2:
				wsprintf(c, L"(%s:%d:%d)(c->s)", pd_sk->wc_ip, pd_sk->i_port, pd_sk->sk);
				break;
			}
			pl->add(c);
		}
		leave();
	}
};