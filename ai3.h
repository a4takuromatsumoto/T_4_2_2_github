#pragma once

#include "INCLUDE/multithread4.h"
#include "INCLUDE/readwrite2.h"
#include "INCLUDE/xrand.h"
#include "INCLUDE/list2.h"
#include <stdio.h>
#include <time.h>

#define NUM_M 4096
class ai3{
public:
	struct _mem{
		byte b_type;
		int i_len;
		wchar_t *pc;
		enum T{
			N = 0,
			I = 1,
			M = 2,
			O = 3
		};
	}mem[NUM_M];
	int i_m;
	struct _logic{
		int i_len, i_len2;
		wchar_t *pc, *pc2;
	};
	list2<_logic> l_l;
	struct _data_calc2{
		wchar_t *pc_v[20], *pc_v2[20];
		int i_num_arg;
		struct _d{
			bool b_reverse;
			wchar_t *pc_arg;
			int i_len_arg;
			byte b_type_arg;
			_d *pd_next, *pd_front;
		}*pd_start, *pd_end;
		bool b_success;

		void clear(){
			_d *pd = pd_start;
			while(pd){
				_d *pd2 = pd->pd_next;
				if(pd->pc_arg)
					delete pd->pc_arg;
				delete pd;
				pd = pd2;
			}
			for(int i = 0; i < 20; i++){
				if(pc_v[i]){
					delete pc_v[i];
				}
			}
		}
		_d *add(){
			_d *pd = new _d();
			ZeroMemory(pd, sizeof(_d));
			if(pd_end){
				pd_end->pd_next = pd;
				pd->pd_front = pd_end;
				pd_end = pd;
			}
			else{
				pd_start = pd;
				pd_end = pd;
			}
			i_num_arg++;
			return pd;
		}
		void init_pc_v2(){
			for(int i = 0; i < 20; i++){
				pc_v2[i] = pc_v[i];
			}
		}
		void f_pc_v2(){
			for(int i = 0; i < 20; i++){
				if(pc_v[i] != pc_v2[i]){
					delete pc_v[i];
					pc_v[i] = pc_v2[i];
				}
			}
		}
	};
	ai3(){
		ZeroMemory(mem, sizeof(mem));
		i_m = 0;
	}
	void set_mem(byte b_type, wchar_t *pc){
		if(mem[i_m].pc)
			delete mem[i_m].pc;

		mem[i_m].b_type = b_type;
		mem[i_m].i_len = wslen(pc);
		mem[i_m].pc = new wchar_t[mem[i_m].i_len + 1];
		wscpy(mem[i_m].pc, pc, mem[i_m].i_len);
		
		i_m = (i_m + 1) % NUM_M;
		if(b_type != _mem::T::O){
			calc1();
			calc2();
		}
	}
	void set_logic(wchar_t *pc, wchar_t *pc2){
		_logic l;
		l.i_len = wslen(pc);
		l.i_len2 = wslen(pc2);
		l.pc = new wchar_t[l.i_len + 1];
		l.pc2 = new wchar_t[l.i_len2 + 1];
		wscpy(l.pc, pc, l.i_len);
		wscpy(l.pc2, pc2, l.i_len2);

		l_l.ins(l);
	}
	void set_file(wchar_t *pc){
		_logic l;
		l.i_len = wslen(pc);
		l.i_len2 = -1;
		l.pc = new wchar_t[l.i_len + 1];
		l.pc2 = NULL;
		wscpy(l.pc, pc, l.i_len);

		l_l.ins(l);
	}
	void calc1(){
		int i_p = 0;
		try{
			an_s_calc1(mem[(i_m + NUM_M - 1) % NUM_M].pc, i_p);
		}
		catch(wchar_t *pc_error){
			MessageBox(NULL, pc_error, L"Error", MB_OK);
		}
	}
	void calc2(){
		_data_calc2 *pdc = new _data_calc2();
		ZeroMemory(pdc, sizeof(_data_calc2));
		int i_m_now = i_m;
		try{
			for(_logic *pl = l_l.end(); pl; pl = l_l.front(pl)){
				if(pl->i_len2 != -1){
					int i_p = 0;
					//i_m_now = i_m;	//’ˆÓ(T_2_8_3)
					an_a_calc2_1(pl->pc, i_p, pdc, i_m_now);
					if(pdc->b_success){
						int i_p2 = 0;
						an_a_calc2_2(pl->pc2, i_p2, pdc);
					}
					pdc->clear();
					ZeroMemory(pdc, sizeof(_data_calc2));
				}
			}
		}
		catch(wchar_t *pc_error){
			//wprintf(pc_error);
			MessageBox(NULL, pc_error, L"Error", MB_OK);
		}
		delete pdc;
	}
	void an_s_calc1(wchar_t *pc, int &i_p){
		int i = 0;
		/*
		if(wscmp(&pc[i_p], L"c(", 2)){
		}
		else{
			for(; pc[i_p + i] != 0 && pc[i_p + i] != ':'; i++){
			}
			if(pc[i_p + i] == ':'){
				i_p += i + 1;
			}
		}
		*/
		if(wscmp(&pc[i_p], L"c(", 2)){
			int i_p1, i_p2, i_p3, i_p4;
			i_p += 2;
			i_p1 = i_p;
			an_a_calc1(pc, i_p);
			i_p2 = i_p;
			match(pc, i_p, L',');
			i_p3 = i_p;
			an_a_calc1(pc, i_p);
			i_p4 = i_p;
			match(pc, i_p, L')');

			wchar_t *pc_1 = new wchar_t[i_p2 - i_p1 + 1];
			wchar_t *pc_2 = new wchar_t[i_p4 - i_p3 + 1];
			wscpy(pc_1, &pc[i_p1], i_p2 - i_p1);
			wscpy(pc_2, &pc[i_p3], i_p4 - i_p3);
			pc_1[i_p2 - i_p1] = 0;
			pc_2[i_p4 - i_p3] = 0;
			set_logic(pc_1, pc_2);
			delete pc_1;
			delete pc_2;
		}
		else if(wscmp(&pc[i_p], L"f(", 2)){
			int i_k = 1;
			i_p += 2;
			int i_p2 = i_p;
			for(; &pc[i_p] != 0; i_p++){
				if(pc[i_p] == L'(')
					i_k++;
				else if(pc[i_p] == L')')
					i_k--;
				if(i_k == 0)
					break;
			}
			wchar_t *pc_file = new wchar_t[i_p - i_p2 + 1];
			wscpy(pc_file, &pc[i_p2], i_p - i_p2);
			pc_file[i_p - i_p2] = 0;
			set_file(pc_file);
			load_logic_no_clear(f_load2(pc_file));
			set_file(L"ret");
			delete pc_file;
		}
		else{
			for(; pc[i_p] != 0; i_p++){

			}
		}
	}
	void an_a_calc1(wchar_t *pc, int &i_p){
		for(; pc[i_p] != L',' && pc[i_p] != L')' && pc[i_p] != 0;){
			an_b_calc1(pc, i_p);
		}
	}
	void an_b_calc1(wchar_t *pc, int &i_p){
		if(wscmp(&pc[i_p], L"!", 1)){
			i_p++;
		}
		if(wscmp(&pc[i_p], L"i", 1)){
			i_p++;
			an_s2(pc, i_p);
			match(pc, i_p, L')');
		}
		else if(wscmp(&pc[i_p], L"m", 1)){
			i_p++;
			an_s2(pc, i_p);
			match(pc, i_p, L')');
		}
		else if(wscmp(&pc[i_p], L"o", 1)){
			i_p++;
			an_s2(pc, i_p);
			match(pc, i_p, L')');
		}
	}
	void an_s2(wchar_t *pc, int &i_p){
		int i_d = 0;
		while(1){
			if(pc[i_p] == L'(')
				i_d++;
			else if(pc[i_p] == L')')
				i_d--;
			if(i_d == 0)
				return;
			i_p++;
		}
	}
	void an_a_calc2_1(wchar_t *pc, int &i_p, _data_calc2 *pdc, int i_m_now2){
		for(; pc[i_p] != 0;){
			an_b_calc2_1(pc, i_p, pdc);
		}
		//
		pdc->b_success = false;
		int i_m_now = (i_m_now2 + NUM_M / 2) % NUM_M;
		int i_m3 = (i_m_now2 + NUM_M - 1) % NUM_M;
		for(_data_calc2::_d *pd = pdc->pd_end; pd; pd = pd->pd_front){
			if(pd == pdc->pd_end){
				if(pd->b_reverse){
					if(pd->b_type_arg == mem[i_m3].b_type && wscmp(pd->pc_arg, mem[i_m3].pc, pdc)){
						return;
					}
				}
				else{
					if(!(pd->b_type_arg == mem[i_m3].b_type && wscmp(pd->pc_arg, mem[i_m3].pc, pdc))){
						return;
					}
				}
				i_m3 = (i_m3 + NUM_M - 1) % NUM_M;
			}
			else{
				if(pd->b_reverse){
					_data_calc2::_d *pd2 = pd;
					bool b_i_m345 = false;
					for(; pd; pd = pd->pd_front){
						if(!(pd->b_reverse)){
							break;
						}
					}
					int i_m4 = i_m3;
					if(pd){
						for(; i_m3 != i_m_now; i_m3 = (i_m3 + NUM_M - 1) % NUM_M){
							if(pd->b_type_arg == mem[i_m3].b_type && wscmp(pd->pc_arg, mem[i_m3].pc, pdc)){
								break;
							}
						}
						if(i_m3 == i_m_now)
							return;
					}
					else{
						i_m3 = (i_m_now + 1) % NUM_M;
						b_i_m345 = true;
					}
					for(_data_calc2::_d *pd3 = pd2; pd3 != pd; pd3 = pd3->pd_front){
						for(int i_m5 = i_m4; i_m5 != i_m3; i_m5 = (i_m5 + NUM_M - 1) % NUM_M){
							if(pd3->b_type_arg == mem[i_m5].b_type && wscmp(pd3->pc_arg, mem[i_m5].pc, pdc)){
								return;
							}
						}
					}
					if(b_i_m345){
						pdc->b_success = true;
						return;
					}
				}
				else{
					for(; i_m3 != i_m_now; i_m3 = (i_m3 + NUM_M - 1) % NUM_M){
						if(pd->b_type_arg == mem[i_m3].b_type && wscmp(pd->pc_arg, mem[i_m3].pc, pdc)){
							break;
						}
					}
				}
			}
			if(i_m3 == i_m_now)
				return;
		}
		pdc->b_success = true;
		return;
	}
	void an_b_calc2_1(wchar_t *pc, int &i_p, _data_calc2 *pdc){
		_data_calc2::_d *pd = pdc->add();
		if(wscmp(&pc[i_p], L"!", 1)){
			pd->b_reverse = true;
			i_p++;
		}
		if(wscmp(&pc[i_p], L"i", 1)){
			int i_p1, i_p2;
			i_p += 1;
			i_p1 = i_p + 1;
			an_s2(pc, i_p);
			i_p2 = i_p;
			match(pc, i_p, L')');

			pd->b_type_arg = _mem::T::I;
			pd->pc_arg = new wchar_t[i_p2 - i_p1 + 1];
			wscpy(pd->pc_arg, &pc[i_p1], i_p2 - i_p1);
		}
		else if(wscmp(&pc[i_p], L"m", 1)){
			int i_p1, i_p2;
			i_p += 1;
			i_p1 = i_p + 1;
			an_s2(pc, i_p);
			i_p2 = i_p;
			match(pc, i_p, L')');

			pd->b_type_arg = _mem::T::M;
			pd->pc_arg = new wchar_t[i_p2 - i_p1 + 1];
			wscpy(pd->pc_arg, &pc[i_p1], i_p2 - i_p1);
		}
		else if(wscmp(&pc[i_p], L"o", 1)){
			int i_p1, i_p2;
			i_p += 1;
			i_p1 = i_p + 1;
			an_s2(pc, i_p);
			i_p2 = i_p;
			match(pc, i_p, L')');

			pd->b_type_arg = _mem::T::O;
			pd->pc_arg = new wchar_t[i_p2 - i_p1 + 1];
			wscpy(pd->pc_arg, &pc[i_p1], i_p2 - i_p1);
		}
	}
	void an_a_calc2_2(wchar_t *pc2, int &i_p, _data_calc2 *pdc){
		for(; pc2[i_p] != 0;){
			an_b_calc2_2(pc2, i_p, pdc);
		}
	}
	void an_b_calc2_2(wchar_t *pc2, int &i_p, _data_calc2 *pdc){
		if(wscmp(&pc2[i_p], L"!", 1)){
			i_p++;
		}
		if(wscmp(&pc2[i_p], L"i", 1)){
			int i_p1, i_p2;
			i_p++;
			i_p1 = i_p + 1;
			an_s2(pc2, i_p);
			i_p2 = i_p;
			match(pc2, i_p, L')');

			wchar_t *pc_buf = new wchar_t[256 * 256 * 256];
			wscpy(pc_buf, &pc2[i_p1], i_p2 - i_p1 , pdc);
			set_mem(_mem::T::I, pc_buf);
			delete pc_buf;
		}
		else if(wscmp(&pc2[i_p], L"m", 1)){
			int i_p1, i_p2;
			i_p++;
			i_p1 = i_p + 1;
			an_s2(pc2, i_p);
			i_p2 = i_p;
			match(pc2, i_p, L')');
			
			wchar_t *pc_buf = new wchar_t[256 * 256 * 256];
			wscpy(pc_buf, &pc2[i_p1], i_p2 - i_p1 , pdc);
			set_mem(_mem::T::M, pc_buf);
			delete pc_buf;
		}
		else if(wscmp(&pc2[i_p], L"o", 1)){
			int i_p1, i_p2;
			i_p++;
			i_p1 = i_p + 1;
			an_s2(pc2, i_p);
			i_p2 = i_p;
			match(pc2, i_p, L')');
			
			wchar_t *pc_buf = new wchar_t[256 * 256 * 256];
			wscpy(pc_buf, &pc2[i_p1], i_p2 - i_p1 , pdc);
			set_mem(_mem::T::O, pc_buf);
			delete pc_buf;
		}
	}
	static void match(wchar_t *pc, int &i_p, wchar_t c){
		if(pc[i_p] == c){
			i_p++;
			return;
		}
		else{
			wchar_t c_buf[256];
			wchar_t c2[2];
			c2[0] = c;
			c2[1] = 0;
			wsprintf(c_buf, L"match error(wchar_t:'%s')", c2);
			throw c_buf;
		}
		return;
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
	static bool wscmp(wchar_t *pc, wchar_t *pc2, _data_calc2 *pdc){
		int i = 0, i2 = 0;
		pdc->init_pc_v2();
		for(; pc[i] != 0; i++){
			if(pc[i] == L'$'){
				int i_v;
				if(pc[i + 1] == L'l'){
					i_v = pc[i + 2] - L'0' + 10;
					i++;
				}
				else{
					i_v = pc[i + 1] - L'0';
				}
				if(i_v < 0 || i_v >= 20)
					throw "wscmp error";
				int i2_2 = i2;
				int i_br = 0;
				//for(; pc[i + 2] != pc2[i2]; i2++){
				if(i_v < 10){
					for(; !sub_wscmp(pc, pc2, i, i2, pdc); i2++){
						if(pc2[i2] == L'(')
							i_br++;
						else if(pc2[i2] == L')')
							i_br--;
						else if(pc2[i2] == 0){	//pc[i + 2] != 0‚¾‚©‚ç‘åä•v‚¾‚¯‚Ç’ˆÓ
							pdc->f_pc_v2();
							return false;
						}
					}
				}
				else{
					i2++;
				}
				if(i_br != 0){
					pdc->f_pc_v2();
					return false;
				}
				if(pdc->pc_v[i_v]){
					if(i2 - i2_2 == 0){
						if(pdc->pc_v[i_v][0] != 0){
							pdc->f_pc_v2();
							return false;
						}
					}
					else if(!wscmp(pdc->pc_v[i_v], &pc2[i2_2], i2 - i2_2)){
						pdc->f_pc_v2();
						return false;
					}
				}
				else{
					//if(i_v >= 10 && i2 - i2_2 != 1){
					//	return false;
					//}
					pdc->pc_v[i_v] = new wchar_t[i2 - i2_2 + 1];
					wscpy(pdc->pc_v[i_v], &pc2[i2_2], i2 - i2_2);
				}
				i++;
			}
			else{
				if(pc[i] == L')')
					return true;
				if(pc[i] != pc2[i2]){
					pdc->f_pc_v2();
					return false;
				}
				i2++;
			}
		}
		if(pc2[i2] != 0){
			pdc->f_pc_v2();
			return false;
		}
		return true;
	}
	static bool sub_wscmp(wchar_t *pc, wchar_t *pc2, int &i, int &i2, _data_calc2 *pdc){
		int j = 0;
		int j2 = 0;
		for(; pc2[i2 + j] != 0/* && pc2[i2 + j] != L'$'*/;){
			if(pc[i + 2 + j2] == L'$')
				break;
			if(pc[i + 2 + j2] != pc2[i2 + j])
				return false;
			j++;
			if(pc[i + 2 + j2] != 0)
				j2++;
		}
		return true;
	}
	static void wscpy(wchar_t *pc, wchar_t *pc2, int i_len){
		for(int i = 0; i < i_len; i++){
			pc[i] = pc2[i];
		}
		pc[i_len] = 0;
	}
	static int wscpy_digit(wchar_t *pc, int i){
		if(i >= 0 && i < 10){
			pc[0] = L'0' + i;
			return 1;
		}
		else if(i >= 10){
			pc[0] = L'0' + (i / 10) % 10;
			pc[1] = L'0' + i % 10;
			return 2;
		}
		return 0;
	}
	static void wscpy(wchar_t *pc, wchar_t *pc2, int i_len, _data_calc2 *pdc){
		int i = 0, i2 = 0;
		for(; i2 < i_len; i2++){
			if(pc2[i2] == L'$'){
				if(pc2[i2 + 1] == L'$'){
					pc[i] = L'$';
					pc[i + 1] = pc2[i2 + 2];
					i += 2;
					i2 += 2;
				}
				else if(pc2[i2 + 1] == L'r'){
					pc[i] = L'0' + xrand () % 10;
					i++;
					i2++;
				}
				else if(pc2[i2 + 1] == L'k'){
					pc[i] = L'±' + xrand() % 45;
					i++;
					i2++;
				}
				else if(pc2[i2 + 1] == L't'){
					time_t t = time(NULL);
					tm *jst = localtime(&t);
					switch(pc2[i2 + 2]){
						case L'0':
							i += wscpy_digit(&(pc[i]), jst->tm_year);
							break;
						case L'1':
							i += wscpy_digit(&(pc[i]), jst->tm_mon + 1);
							break;
						case L'2':
							i += wscpy_digit(&(pc[i]), jst->tm_mday);
							break;
						case L'3':
							i += wscpy_digit(&(pc[i]), jst->tm_wday);
							break;
						case L'4':
							i += wscpy_digit(&(pc[i]), jst->tm_hour);
							break;
						case L'5':
							i += wscpy_digit(&(pc[i]), jst->tm_min);
							break;
						case L'6':
							i += wscpy_digit(&(pc[i]), jst->tm_sec);
							break;
					}
					//i += 2;
					i2 += 2;
					//pc[i] = 0;
				}
				else if(pc2[i2 + 1] == L'l'){
					int i_v = pc2[i2 + 2] - L'0' + 10;
					if(i_v >= 20)
						throw "wscpy error";
					int i_2 = i;
					for(; pdc->pc_v[i_v][i - i_2] != 0; i++){
						pc[i] = pdc->pc_v[i_v][i - i_2];
					}
					i2 += 2;
				}
				else{
					int i_v = pc2[i2 + 1] - L'0';
					if(i_v >= 10)
						throw "wscpy error";
					int i_2 = i;
					for(; pdc->pc_v[i_v][i - i_2] != 0; i++){
						pc[i] = pdc->pc_v[i_v][i - i_2];
					}
					i2++;
				}
			}
			else{
				pc[i] = pc2[i2];
				i++;
			}
		}
		pc[i] = 0;
	}
	void print_mem(_edit *pe){
		int i_c = 0;
		int i_num = 0;
		int i_len_all = 0;
		pe->set(L"");
		for(int k = 0; k < NUM_M; k++){
			int i = (i_m + k) % NUM_M;
			if(mem[i].b_type != _mem::T::N){
				i_len_all += mem[i].i_len + 16;
			}
		}
		if(i_len_all > 0){
			wchar_t *pc = new wchar_t[i_len_all];
			for(int k = 0; k < NUM_M; k++){
				int i = (i_m + k) % NUM_M;
				if(mem[i].b_type != _mem::T::N){
					wchar_t c2[16];

					switch(mem[i].b_type){
						case _mem::T::I:
							wsprintf(c2, L"[%d](i)", i);
							break;
						case _mem::T::M:
							wsprintf(c2, L"[%d](m)", i);
							break;
						case _mem::T::O:
							wsprintf(c2, L"[%d](o)", i);
							break;
					}
					for(int j = 0; c2[j] != 0; j++){
						pc[i_c] = c2[j];
						i_c++;
					}
					for(int j = 0; mem[i].pc[j] != 0; j++){
						pc[i_c] = mem[i].pc[j];
						i_c++;
					}
					pc[i_c] = '\r';
					i_c++;
					pc[i_c] = '\n';
					i_c++;
					i_num++;
				}
			}
			pc[i_c] = 0;
			pe->set(pc);
			delete pc;
			for(int i = 0; i < i_num; i++)
				pe->scrolldown();
		}
	}
	void print_logic(_edit *pe){
		
		int i_len_all = 0;
		pe->set(L"");
		
		for(_logic *pl = l_l.start(); pl; pl = l_l.next(pl)){
			if(pl->i_len2 == -1){
				i_len_all += pl->i_len + 16;
			}
			else{
				i_len_all += pl->i_len + pl->i_len2 + 16;
			}
		}
		if(i_len_all > 0){
			wchar_t *pc = new wchar_t[i_len_all];
			int i_c = 0;
			int i_num = 0;
			for(_logic *pl = l_l.start(); pl; pl = l_l.next(pl)){
				if(pl->i_len2 == -1){
					wchar_t c2[16];
					wsprintf(c2, L"[%d]file:", i_num);
					for(int j = 0; c2[j] != 0; j++){
						pc[i_c] = c2[j];
						i_c++;
					}
					for(int j = 0; pl->pc[j] != 0; j++){
						pc[i_c] = pl->pc[j];
						i_c++;
					}
					pc[i_c] = '\r';
					i_c++;
					pc[i_c] = '\n';
					i_c++;
					i_num++;
				}
				else{
					wchar_t c2[16];
					wsprintf(c2, L"[%d]", i_num);
					for(int j = 0; c2[j] != 0; j++){
						pc[i_c] = c2[j];
						i_c++;
					}
					for(int j = 0; pl->pc[j] != 0; j++){
						pc[i_c] = pl->pc[j];
						i_c++;
					}
					pc[i_c] = '-';
					i_c++;
					pc[i_c] = '>';
					i_c++;
					for(int j = 0; pl->pc2[j] != 0; j++){
						pc[i_c] = pl->pc2[j];
						i_c++;
					}
					pc[i_c] = '\r';
					i_c++;
					pc[i_c] = '\n';
					i_c++;
					i_num++;
				}
			}
			pc[i_c] = 0;
			pe->set(pc);
			for(int i = 0; i < i_num; i++)
				pe->scrolldown();
			delete pc;
		}
	}
	void load_logic_no_clear(f_load2 &fl){
		//—Ìˆæ‚Ì‘å‚«‚³’ˆÓ
		wchar_t *pc_buf = new wchar_t[256 * 256 * 256];
		wchar_t *pc_buf2 = new wchar_t[256 * 256 * 256];
		int i_len = fl((char *)pc_buf);
		pc_buf[i_len / 2] = 0;
		if(pc_buf[0] != 0xfeff){
			printf("unicode error\n");
		}
		int i2 = 0;
		for(int i = 1; pc_buf[i] != 0; i++){
			if(pc_buf[i] == L'\n'){
				pc_buf2[i2] = 0;
				set_mem(_mem::T::I, pc_buf2);
				i2 = 0;
				continue;
			}
			else if(pc_buf[i] == L'\r'){
			}
			else if(pc_buf[i + 1] == 0){
				pc_buf2[i2] = pc_buf[i];
				pc_buf2[i2 + 1] = 0;
				set_mem(_mem::T::I, pc_buf2);
				i2 = 0;
				continue;
			}
			else{
				pc_buf2[i2] = pc_buf[i];
				i2++;
			}
		}
		delete pc_buf;
		delete pc_buf2;
	}
	void load_logic(f_load2 &fl){
		l_l.clear();
		load_logic_no_clear(fl);
	}
	void save_logic(f_save2 &fs){
		_logic *pl = l_l.start();
		save_logic(fs, pl);
	}
	void save_logic(f_save2 &fs, _logic *&pl){
		short s = 0xfeff;
		fs(s);
		for(; pl; pl = l_l.next(pl)){
			if(pl->i_len2 == -1){
				if(wscmp(pl->pc, L"ret", 3)){
					return;
				}
				fs((char *)L"f(", 4);
				for(int j = 0; pl->pc[j] != 0; j++){
					fs(pl->pc[j]);
				}
				fs((char *)L")", 2);
				fs(0x000A000D);
				int i_save = 0;
				if(ws::wslen(pl->pc) >= 5){
					if(ws::wscmp(pl->pc, L"Down/", 5)){
						i_save++;
					}
				}
				wchar_t *pc_name = pl->pc;
				pl = l_l.next(pl);
				if(pl == NULL)
					break;
				if(i_save == 0){
					save_logic(f_save2(pc_name), pl);
				}
				else if(i_save > 0)
					sub_save_logic(pl, i_save);
			}
			else{
				fs((char *)L"c(", 4);
				for(int j = 0; pl->pc[j] != 0; j++){
					fs(pl->pc[j]);
				}
				fs((char *)L",", 2);
				for(int j = 0; pl->pc2[j] != 0; j++){
					fs(pl->pc2[j]);
				}
				fs((char *)L")", 2);
				fs(0x000A000D);
			}
		}
	}
	void sub_save_logic(_logic *&pl, int i_save){
		for(; pl; pl = l_l.next(pl)){
			if(pl->i_len2 == -1){
				if(wscmp(pl->pc, L"ret", 3)){
					if(i_save > 0)
						i_save--;
					return;
				}
				if(ws::wscmp(pl->pc, L"Down/", 5)){
					i_save++;
				}
				sub_save_logic(pl, i_save);
			}
		}
	}
	bool get_out(wchar_t *pc, int i_len, int i_m3){
		if(mem[i_m3 % NUM_M].b_type == _mem::T::O){
			for(int i = 0; i < i_len; i++){
				if(mem[i_m3 % NUM_M].pc[i] == 0){
					pc[i] = 0;
					break;
				}
				else{
					pc[i] = mem[i_m3 % NUM_M].pc[i];
				}
			}
			return true;
		}
		return false;
	}
};