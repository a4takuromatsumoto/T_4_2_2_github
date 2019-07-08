#pragma once

#include "INCLUDE/list2.h"
#include "INCLUDE/socket2.h"
#include "INCLUDE/multithread4.h"

class filelist{
public:
	struct _data{
		wchar_t c_name[256];
		SOCKET s;
		int i_state;
	};
	list2<_data> l_d;
	filelist(){
	}
	void ins(wchar_t *pc, SOCKET s){
		_data *pd_search;
		bool b_exist = false;
		for(pd_search = l_d.start(); pd_search; pd_search = l_d.next(pd_search)){
			if(ws::wscmp2(pd_search->c_name, pc, 256) && pd_search->s == s){
				b_exist = true;
				break;
			}
		}
		if(!b_exist){
			_data d;
			ws::wscpy(d.c_name, pc, 256);
			d.s = s;
			d.i_state = 0;
			l_d.ins(d);
		}
	}
	void del(SOCKET s){
		_data *pd = l_d.start();
		while(pd){
			_data *pd2 = l_d.next(pd);
			if(pd->s == s){
				l_d.del(pd);
			}
			pd = pd2;
		}
	}
	void print(_list *pl){
		pl->reset();
		for(_data *pd = l_d.start(); pd; pd = l_d.next(pd)){
			wchar_t c_buf[1024];
			c_buf[0] = 0;
			//if(pd->i_state == 0){
				wsprintf(c_buf, L"%s", pd->c_name);
			//}
			//else if(pd->i_state == 1){
			//	wsprintf(c_buf, L"%s(down’†)", pd->c_name);
			//}
			pl->add(c_buf);
		}
	}
	_data *search(int i){
		int j = 0;
		for(_data *pd = l_d.start(); pd; pd = l_d.next(pd)){
			if(j == i)
				return pd;
			j++;
		}
		return NULL;
	}
	_data *search(wchar_t *pc_name){
		for(_data *pd = l_d.start(); pd; pd = l_d.next(pd)){
			if(ws::wscmp(pd->c_name, pc_name, ws::wslen(pd->c_name))){
				return pd;
			}
		}
		return NULL;
	}
};