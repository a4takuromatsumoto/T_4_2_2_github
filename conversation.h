#pragma once

#include "INCLUDE/multithread4.h"

class conversation{
public:
	wchar_t c[65536];
	int i_p;
	int i_r;
	conversation(){
		ZeroMemory(c, sizeof(c));
		i_p = 0;
		i_r = 0;
	}
	void set(wchar_t *pc, _edit *pe){
		for(int i = 0; i < 65536; i++){	//256’ˆÓ
			c[i_p] = pc[i];
			if(c[i_p] == 0){
				c[i_p] = 0x000d;
				c[i_p + 1] = 0x000a;
				i_p += 2;
				break;
			}
			i_p++;
			if(i_p >= 65534)
				return;
		}
		i_r++;
		pe->set(c);
		for(int i = 0; i < i_r; i++){
			pe->scrolldown();
		}
	}
	void set_noprint(wchar_t *pc){
		for(int i = 0; i < 65536; i++){	//256’ˆÓ
			c[i_p] = pc[i];
			if(c[i_p] == 0){
				c[i_p] = 0x000d;
				c[i_p + 1] = 0x000a;
				i_p += 2;
				break;
			}
			i_p++;
			if(i_p >= 65534)
				return;
		}
		i_r++;
	}
	void print(_edit *pe){
		pe->set(c);
	}
};