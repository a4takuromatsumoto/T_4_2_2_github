#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <math.h>
#include "INCLUDE/socket2.h"
#include "socketlist.h"
typedef unsigned char byte;
#include "INCLUDE/multithread4.h"
#include "cai3.h"
#include "conversation.h"
#include "filelist.h"
#pragma comment(lib, "wsock32.lib")

#define _UNICDOE

#define ID_TIMER1 100
multithread mt;
HANDLE h_endofcreatewindow;
HANDLE h_destroy;
window::_procedure::_staticwindow2 *psw[8];
cai3 cai;
_edit *pe[16];
_tab *ptb[8];
_button *pb[8];
_list *pl[8];
int i_h_tr;

//int i_state_net;
socketclass sc;
socketlist sl;
conversation cvs;	//同期注意
filelist fl;

wchar_t c_name_c[256];

enum{
	TM_EDITRETURN = 0x100,
	TM_EDITRETURN2 = 0x101,
	TM_TIMER = 0x200,
	TM_FADD = 0x300,
	TM_UP = 0x400,
	TM_CONNECT = 0x500
};
void print_up(){
	pl[2]->reset();
	struct _wfinddata_t fdata;

	int fh = _wfindfirst(L"./Up/*.*", &fdata);
	if(fh == -1){
	}
	else{
		do{
			if((fdata.attrib & _A_SUBDIR) == 0){
				wchar_t c_name[1024];
				//mbstowcs(c_name, fdata.name, strlen(fdata.name) + 1);
				pl[2]->add(fdata.name);
			}
		}while(_wfindnext(fh, &fdata) == 0);
		//_wfindclose(fh);
	}
}
void print_down(){
	pl[3]->reset();
	struct _wfinddata_t fdata2;
	intptr_t fh2 = _wfindfirst(L"./Down/*.*", &fdata2);
	if(fh2 == -1){
	}
	else{
		do{
			if((fdata2.attrib & _A_SUBDIR) == 0){
				wchar_t c_name[1024];
				//mbstowcs(c_name, fdata2.name, strlen(fdata2.name) + 1);
				pl[3]->add(fdata2.name);
			}
		}while(_wfindnext(fh2, &fdata2) == 0);
		//_wfindclose(fh2);
	}
}
struct _tr_client : public multithread::_thread{
	wchar_t wc_ip[256];
	char c_ip[256];
	wchar_t c1[256], c2[256];
	int i_port;
	int i_ip;
	_tr_client(wchar_t *pwc_ip, wchar_t *pc1){
		ws::wscpy(wc_ip, pwc_ip, 256);
		ws::wscpy(c1, pc1, 256);
	}
	void operator()(){
		//i_state_net = 1;
		//printf("%s\n", name);
		WaitForSingleObject(h_endofcreatewindow, INFINITE);
		try{
			c_ip[255] = 0;
			i_ip = ipfromstring(wc_ip);
			stringfromip(c_ip, i_ip);
			c1[255] = 0;
			i_port = _wtoi(c1);
			//wsprintf(c2, L"サーバーへ接続(%s:%d)", wc_ip, i_port);
			//MessageBox(NULL, c2, L"Message", MB_OK);
			SOCKET s = sc.getsocket_client(c_ip, i_port);
			//sl_s.ins(s, (int)ipfromstring(c_ip), i_port);
			//sl_s.set(pl[0]);
			//wsprintf(c2, L"接続成功しました。", i_port);
			//MessageBox(NULL, c2, L"Message", MB_OK);
			{
				wchar_t c2[256];
				wsprintf(c2, L"%s:%d", wc_ip, i_port);
				int i_len = ws::wslen(c2);
				wchar_t *pc_buf = new wchar_t[i_len + 3];
				*(int *)(pc_buf) = TM_CONNECT;
				ws::wscpy(&pc_buf[2], c2, i_len);
				pc_buf[i_len + 2] = 0;

				send(L"thread", pc_buf, (i_len + 3) * 2, true);
			}


			socketlist::_data_sk *pd_sk = sl.ins(s, wc_ip, i_port, 2);
			sl.set(pl[0]);
			pl[0]->setcursel(pl[0]->getcount() - 1);
			pe[6]->set(L"");
			
			struct _wfinddata_t fdata;
			
			int fh = _wfindfirst(L"./Up/*.*", &fdata);
			if(fh == -1){
			}
			else{
				do{
					if((fdata.attrib & _A_SUBDIR) == 0){
						sc.Send(s, "fadd", 4);
						wchar_t c_name[1024];
						int i_len = ws::wslen(fdata.name) + 1;
						int i_len2 = i_len * 2;
						sc.Send(s, (char *)&i_len2, 4);
						//mbstowcs(c_name, fdata.name, i_len);
						sc.Send(s, (char *)fdata.name, i_len2);
					}
				}while(_wfindnext(fh, &fdata) == 0);
				//_findclose(fh);
			}

			while(1){
				char c_sig[4];

				sc.Recv(s, (char *)c_sig, 4);
				if(!strncmp(c_sig, "del ", 4)){
					socketlist::_data_sk *pd_sk = sl.search_d(s);
					if(pd_sk){
						fl.del(s);
						fl.print(pl[1]);

						sl.l_d_sk.del(pd_sk);
						sl.set(pl[0]);
						break;
					}
				}
				else if(!strncmp(c_sig, "stc ", 4)){
					int i_n;
					sc.Recv(s, (char *)&i_n, 4);
					if(i_n > 65536 || i_n < 0){
						MessageBox(NULL, L"i_num_data error", L"Message", MB_OK);
						break;
					}
					char *pc = new char[i_n];
					sc.Recv(s, pc, i_n);

				
					socketlist::_data_sk *pd_sk = sl.search_d(s);
					conversation *pcv = pd_sk->pcv;

					wchar_t c_buf[65536];
					wsprintf(c_buf, L"相手：%s", pc);
					pcv->set_noprint(c_buf);
				
					int i_cursel = pl[0]->getcursel();
					if(i_cursel >= 0){
						socketlist::_data_sk *pd_sk = sl.search_ar(i_cursel);
						if(pd_sk){
							pd_sk->pcv->print(pe[6]);
						}
					}
					delete pc;
				}
				else if(!strncmp(c_sig, "updt", 4)){
					struct _wfinddata_t fdata;
			
					int fh = _wfindfirst(L"./Up/*.*", &fdata);
					if(fh == -1){
					}
					else{
						do{
							if((fdata.attrib & _A_SUBDIR) == 0){
								sc.Send(s, "fadd", 4);
								wchar_t c_name[1024];
								int i_len = ws::wslen(fdata.name) + 1;
								int i_len2 = i_len * 2;
								sc.Send(s, (char *)&i_len2, 4);
								//mbstowcs(c_name, fdata.name, i_len);
								sc.Send(s, (char *)fdata.name, i_len2);
							}
						}while(_wfindnext(fh, &fdata) == 0);
						//_findclose(fh);
					}
				}
				else if(!strncmp(c_sig, "fadd", 4)){
					int i_n2;
					sc.Recv(s, (char *)&i_n2, 4);
					if(i_n2 > 65536 || i_n2 <= 0){
						MessageBox(NULL, L"i_num_data error", L"Message", MB_OK);
						break;
					}
					char *pc2 = new char[i_n2];
					sc.Recv(s, pc2, i_n2);
					
					wchar_t c_buf[65536];
					wsprintf(c_buf, L"%s", pc2);
					fl.ins(c_buf, s);

					int i_len = ws::wslen(c_buf);
					wchar_t *pc_buf = new wchar_t[i_len + 3];
					*(int *)(pc_buf) = TM_FADD;
					ws::wscpy(&pc_buf[2], c_buf, i_len);
					pc_buf[i_len + 2] = 0;

					send(L"thread", pc_buf, (i_len + 3) * 2, true);
					fl.print(pl[1]);
					delete pc2;
				}
				else if(!strncmp(c_sig, "down", 4)){
					int i_n2;
					sc.Recv(s, (char *)&i_n2, 4);
					if(i_n2 > 65536 || i_n2 <= 0){
						MessageBox(NULL, L"i_num_data error", L"Message", MB_OK);
						break;
					}
					char *pc2 = new char[i_n2];
					sc.Recv(s, pc2, i_n2);
										
					wchar_t c_buf[65536];
					wchar_t c_buf_2[65536];
					//char c_buf2[65536 * 256];
					wsprintf(c_buf, L"./Up/%s", pc2);
					wsprintf(c_buf_2, L"%s", pc2);

					f_load2 fl(c_buf);
					char *pc_buf2 = new char[65536 * 16384];
					int i_n3 = fl.load_all(pc_buf2, 65536 * 16384);
					sc.Send(s, "up  ", 4);
					sc.Send(s, (char *)&i_n2, 4);
					sc.Send(s, (char *)c_buf_2, i_n2);
					sc.Send(s, (char *)&i_n3, 4);
					if(i_n3 > 0)
						sc.Send(s, pc_buf2, i_n3);
					delete pc2;
					delete pc_buf2;
				}
				else if(!strncmp(c_sig, "up  ", 4)){
					int i_n2, i_n3;
					sc.Recv(s, (char *)&i_n2, 4);
					if(i_n2 > 65536 || i_n2 < 0){
						MessageBox(NULL, L"i_num_data error", L"Message", MB_OK);
						break;
					}
					char *pc2 = new char[i_n2];
					if(i_n2 > 0)
						sc.Recv(s, pc2, i_n2);
										
					wchar_t c_buf[65536];
					//char c_buf2[65536 * 256];
					wsprintf(c_buf, L"./Down/%s", pc2);
					
					sc.Recv(s, (char *)&i_n3, 4);

					char *pc3 = new char[i_n3];
					if(i_n3 > 0)
						sc.Recv(s, pc3, i_n3);
					
					char *pc_buf2 = new char[65536 * 16384];
					memcpy(pc_buf2, pc3, i_n3);

					f_save2 fs(c_buf);
					fs(pc_buf2, i_n3);

					print_down();

					int i_len = ws::wslen(c_buf);
					wchar_t *pc_buf = new wchar_t[i_len + 3];
					*(int *)(pc_buf) = TM_UP;
					ws::wscpy(&pc_buf[2], c_buf, i_len);
					pc_buf[i_len + 2] = 0;

					send(L"thread", pc_buf, (i_len + 3) * 2, true);
					delete pc2;
					delete pc3;
					delete pc_buf2;
				}
			}
		}
		catch(wchar_t *pc_error){
			//printf("%s\n", pc_error);
		}
	}
};
struct _tr_server_child : public multithread::_thread{
	SOCKET s;
	wchar_t c_ip[256];
	int i_port;
	_tr_server_child(SOCKET s, wchar_t *pc_ip, int i_port){
		this->s = s;
		wsprintf(c_ip, L"%s", pc_ip);
		this->i_port = i_port;
	}
	void operator()(){
		WaitForSingleObject(h_endofcreatewindow, INFINITE);
		
		try{
			while(1){
				char c_sig[4];
				sc.Recv(s, (char *)c_sig, 4);
				if(!strncmp(c_sig, "del ", 4)){
					socketlist::_data_sk *pd_sk = sl.search_d(s);
					if(pd_sk){
						fl.del(s);
						fl.print(pl[1]);

						sl.l_d_sk.del(pd_sk);
						sl.set(pl[0]);
						break;
					}
				}
				else if(!strncmp(c_sig, "stc ", 4)){
					int i_n;
					sc.Recv(s, (char *)&i_n, 4);
					if(i_n > 65536 || i_n < 0){
						MessageBox(NULL, L"i_num_data error", L"Message", MB_OK);
						break;
					}
					char *pc1 = new char[i_n];
					sc.Recv(s, pc1, i_n);

				
					socketlist::_data_sk *pd_sk = sl.search_d(s);
					conversation *pcv = pd_sk->pcv;

					wchar_t c_buf[65536];
					wsprintf(c_buf, L"相手：%s", pc1);
					pcv->set_noprint(c_buf);
				
					int i_cursel = pl[0]->getcursel();
					if(i_cursel >= 0){
						socketlist::_data_sk *pd_sk = sl.search_ar(i_cursel);
						if(pd_sk){
							pd_sk->pcv->print(pe[6]);
						}
					}
					delete pc1;
				}
				else if(!strncmp(c_sig, "updt", 4)){
					struct _wfinddata_t fdata;
			
					int fh = _wfindfirst(L"./Up/*.*", &fdata);
					if(fh == -1){
					}
					else{
						do{
							if((fdata.attrib & _A_SUBDIR) == 0){
								sc.Send(s, "fadd", 4);
								wchar_t c_name[1024];
								int i_len = ws::wslen(fdata.name) + 1;
								int i_len2 = i_len * 2;
								sc.Send(s, (char *)&i_len2, 4);
								//mbstowcs(c_name, fdata.name, i_len);
								sc.Send(s, (char *)fdata.name, i_len2);
							}
						}while(_wfindnext(fh, &fdata) == 0);
						//_findclose(fh);
					}
				}
				else if(!strncmp(c_sig, "fadd", 4)){
					int i_n2;
					sc.Recv(s, (char *)&i_n2, 4);
					if(i_n2 > 65536 || i_n2 <= 0){
						MessageBox(NULL, L"i_num_data error", L"Message", MB_OK);
						break;
					}
					char *pc2 = new char[i_n2];
					sc.Recv(s, pc2, i_n2);
					
					wchar_t c_buf[65536];
					wsprintf(c_buf, L"%s", pc2);
					fl.ins(c_buf, s);

					int i_len = ws::wslen(c_buf);
					wchar_t *pc_buf = new wchar_t[i_len + 3];
					*(int *)(pc_buf) = TM_FADD;
					ws::wscpy(&pc_buf[2], c_buf, i_len);
					pc_buf[i_len + 2] = 0;

					send(L"thread", pc_buf, (i_len + 3) * 2, true);
					fl.print(pl[1]);
					delete pc2;
				}
				else if(!strncmp(c_sig, "down", 4)){
					int i_n2;
					sc.Recv(s, (char *)&i_n2, 4);
					if(i_n2 > 65536 || i_n2 <= 0){
						MessageBox(NULL, L"i_num_data error", L"Message", MB_OK);
						break;
					}
					char *pc2 = new char[i_n2];
					sc.Recv(s, pc2, i_n2);
										
					wchar_t c_buf[65536];
					wchar_t c_buf_2[65536];
					//char c_buf2[65536 * 256];
					wsprintf(c_buf, L"./Up/%s", pc2);
					wsprintf(c_buf_2, L"%s", pc2);
					
					f_load2 fl(c_buf);
					char *pc_buf2 = new char[65536 * 16384];
					int i_n3 = fl.load_all(pc_buf2, 65536 * 16384);
					sc.Send(s, "up  ", 4);
					sc.Send(s, (char *)&i_n2, 4);
					sc.Send(s, (char *)c_buf_2, i_n2);
					sc.Send(s, (char *)&i_n3, 4);
					if(i_n3 > 0)
						sc.Send(s, pc_buf2, i_n3);
					delete pc2;
					delete pc_buf2;
				}
				else if(!strncmp(c_sig, "up  ", 4)){
					int i_n2, i_n3;
					sc.Recv(s, (char *)&i_n2, 4);
					if(i_n2 > 65536 || i_n2 < 0){
						MessageBox(NULL, L"i_num_data error", L"Message", MB_OK);
						break;
					}
					char *pc2 = new char[i_n2];
					if(i_n2 > 0)
						sc.Recv(s, pc2, i_n2);
										
					wchar_t c_buf[65536];
					//char c_buf2[65536 * 256];
					wsprintf(c_buf, L"./Down/%s", pc2);
					
					sc.Recv(s, (char *)&i_n3, 4);

					char *pc3 = new char[i_n3];
					if(i_n3 > 0)
						sc.Recv(s, pc3, i_n3);
					
					char *pc_buf2 = new char[65536 * 16384];
					memcpy(pc_buf2, pc3, i_n3);

					f_save2 fs(c_buf);
					fs(pc_buf2, i_n3);

					print_down();

					int i_len = ws::wslen(c_buf);
					wchar_t *pc_buf = new wchar_t[i_len + 3];
					*(int *)(pc_buf) = TM_UP;
					ws::wscpy(&pc_buf[2], c_buf, i_len);
					pc_buf[i_len + 2] = 0;

					send(L"thread", pc_buf, (i_len + 3) * 2, true);
					delete pc2;
					delete pc3;
					delete pc_buf2;
				}
			}
		}
		catch(wchar_t *pc_error){
			//printf("%s\n", pc_error);
		}
	}
};
struct _tr_server : public multithread::_thread{
	int i_port;
	_tr_server(int i_port){
		this->i_port = i_port;
	}
	void operator()(){
		printf("%s\n", name);
		WaitForSingleObject(h_endofcreatewindow, INFINITE);
		socketclass sc;
		try{
			wchar_t c_ip[256], c2[256];
			wsprintf(c2, L"ポート番号(%d)でサーバーが立ち上がりました。\n", i_port);
			MessageBox(NULL, c2, L"Message", MB_OK);
			//SOCKET listen_s = sc.getsocket_server1();
			while(1){
				//wsprintf(c2, L"接続待ち。\n", i_port);
				//MessageBox(NULL, c2, L"Message", MB_OK);
				//SOCKET s = sc.getsocket_server2(listen_s, i_port);
				socketclass::_clientdata cd = sc.getsocket_server(i_port);
				SOCKET s = cd.s;
				int i_ip = (int)cd.d;
				stringfromip(c_ip, i_ip);
				//wsprintf(c2, L"クライアントが来ました。(%s:%d)\n", c_ip, i_port);
				//MessageBox(NULL, c2, L"Message", MB_OK);
				
				socketlist::_data_sk *pd_sk = sl.ins(s, c_ip, i_port, 1);
				sl.set(pl[0]);
				pl[0]->setcursel(pl[0]->getcount() - 1);
				pe[6]->set(L"");
				
				wsprintf(c2, L"%s:%d", c_ip, i_port);
				int i_len = ws::wslen(c2);
				wchar_t *pc_buf = new wchar_t[i_len + 3];
				*(int *)(pc_buf) = TM_CONNECT;
				ws::wscpy(&pc_buf[2], c2, i_len);
				pc_buf[i_len + 2] = 0;

				send(L"thread", pc_buf, (i_len + 3) * 2, true);

				struct _wfinddata_t fdata;
			
				int fh = _wfindfirst(L"./Up/*.*", &fdata);
				if(fh == -1){
				}
				else{
					do{
						if((fdata.attrib & _A_SUBDIR) == 0){
							sc.Send(s, "fadd", 4);
							wchar_t c_name[1024];
							int i_len = ws::wslen(fdata.name) + 1;
							int i_len2 = i_len * 2;
							sc.Send(s, (char *)&i_len2, 4);
							//mbstowcs(c_name, fdata.name, i_len);
							sc.Send(s, (char *)fdata.name, i_len2);
						}
					}while(_wfindnext(fh, &fdata) == 0);
					//_findclose(fh);
				}

				wchar_t c3[256];
				wsprintf(c3, L"server_child(%d)", i_h_tr);
				i_h_tr++;
				mt.begin(new _tr_server_child(s, c_ip, i_port), c3);
				//SendMessage(hwnd_base, MM_WIM_DATA, 0, 0);
				//l_s.ins(s);
			}
		}
		catch(char *pc_error){
			//printf("%s\n", pc_error);
		}

		while(1){
			if(!iszero()){
				int *pi = (int *)peek()->body.pc;
				switch(*pi){
					case TM_EDITRETURN:
						{
							char *pc = &peek()->body.pc[4];
						}
						break;
				}
				pop();
			}
			else{
				wait(1000);
			}
		}
	}
};
struct _proc : public window::_procedure{
	multithread::_thread *ptr;
	void operator()(window::_arg &arg){
		switch(arg.uMsg){
			case WM_COMMAND:
				{
					if(arg.lp == (LPARAM)pb[0]->h){
						wchar_t c_port[256];
						pe[5]->get(c_port, 256);
						c_port[255] = 0;
						int i_port = _wtoi(c_port);
						mt.begin(new _tr_server(i_port), L"server");
					}
					if(arg.lp == (LPARAM)pb[1]->h){
						wchar_t wc_ip[256], c1[256];
						pe[4]->get(wc_ip, 256);
						pe[5]->get(c1, 256);

						mt.begin(new _tr_client(wc_ip, c1), L"client");
					}
					if(arg.lp == (LPARAM)pb[2]->h){
						int i_c = pl[1]->getcursel();
						if(i_c >= 0){
							filelist::_data *pd = fl.search(i_c);
							if(pd){
								pd->i_state = 1;
								fl.print(pl[1]);
								sc.Send(pd->s, "down", 4);
								int i_n = ws::wslen(pd->c_name) * 2 + 2;
								sc.Send(pd->s, (char *)&i_n, 4);
								sc.Send(pd->s, (char *)pd->c_name, i_n);
							}
						}
					}
					if(arg.lp == (LPARAM)pb[3]->h){
						print_up();
						print_down();
						for(socketlist::_data_sk *pd_sk = sl.l_d_sk.start(); pd_sk; pd_sk = sl.l_d_sk.next(pd_sk)){
							//受信
							sc.Send(pd_sk->sk, "updt", 4);
							//送信
							struct _wfinddata_t fdata;
			
							int fh = _wfindfirst(L"./Up/*.*", &fdata);
							if(fh == -1){
							}
							else{
								do{
									if((fdata.attrib & _A_SUBDIR) == 0){
										sc.Send(pd_sk->sk, "fadd", 4);
										wchar_t c_name[1024];
										int i_len = ws::wslen(fdata.name) + 1;
										int i_len2 = i_len * 2;
										sc.Send(pd_sk->sk, (char *)&i_len2, 4);
										//mbstowcs(c_name, fdata.name, i_len);
										sc.Send(pd_sk->sk, (char *)fdata.name, i_len2);
									}
								}while(_wfindnext(fh, &fdata) == 0);
								//_findclose(fh);
							}
						}
					}
					if(HIWORD(arg.wp) == LBN_SELCHANGE){
						int i_cursel = pl[0]->getcursel();
						if(i_cursel >= 0){
							socketlist::_data_sk *pd_sk = sl.search_ar(i_cursel);
							if(pd_sk){
								pd_sk->pcv->print(pe[6]);
							}
						}
					}
				}
				break;
			case WM_QUEUEMESSAGE:
				{
					multithread::_data *pd = ptr->peek();
					ptr->pop();
				}
				break;
			case WM_TIMER:
				switch(arg.wp){
					case ID_TIMER1:
					{
						char *pc = new char[5];
						((int *)pc)[0] = TM_TIMER;
						pc[4] = 0;
						ptr->send(L"thread", pc, 5, true);
					}
					break;
				}
				break;
			case WM_EDITRETURN:
				{
					if((_edit *)arg.wp == pe[1]){
						char *pc = new char[5];
						((int *)pc)[0] = TM_EDITRETURN;
						pc[4] = 0;
						ptr->send(L"thread", pc, 5, true);
					}
					if((_edit *)arg.wp == pe[7]){
						char *pc = new char[5];
						((int *)pc)[0] = TM_EDITRETURN2;
						pc[4] = 0;
						ptr->send(L"thread", pc, 5, true);
					}
				}
				break;
			case WM_NOTIFY:
				{
					switch(((NMHDR *)(arg.lp))->code){
					case TCN_SELCHANGE:
						if(ptb[3]->get() == 0){
							psw[0]->show();
							pl[4]->hide();
						}
						else{
							psw[0]->hide();
							pl[4]->show();
						}
						switch(ptb[0]->get()){
						case 0:
							ptb[1]->show();
							ptb[2]->hide();

							psw[1]->hide();
							psw[2]->hide();
							
							switch(ptb[1]->get()){
							case 0:
								if(ptb[3]->get() == 0){
									pe[2]->show();
									pe[3]->show();
								}
								else{
									pe[2]->hide();
									pe[3]->hide();
								}
								break;
							case 1:
								pe[2]->hide();
								pe[3]->hide();
								break;
							case 2:
								//cai.pc3->set_circle();
								//cai.pc3->save();
								pe[2]->hide();
								pe[3]->hide();
								break;
							}
							pb[0]->hide();
							pb[1]->hide();

							pe[4]->hide();
							pe[5]->hide();

							pe[6]->hide();
							pe[7]->hide();

							pl[0]->hide();
							pl[1]->hide();
							pl[2]->hide();
							pl[3]->hide();

							psw[3]->hide();
							psw[4]->hide();

							pb[2]->hide();
							pb[3]->hide();
							break;
						case 1:
							ptb[1]->hide();
							ptb[2]->show();

							pe[2]->hide();
							pe[3]->hide();

							switch(ptb[2]->get()){
							case 0:
								pe[4]->show();
								pe[5]->show();

								psw[1]->show();
								psw[2]->show();

								pb[0]->show();
								pb[1]->show();

								pe[6]->show();
								pe[7]->show();

								pl[0]->show();

								pl[1]->hide();
								pl[2]->hide();
								pl[3]->hide();

								psw[3]->hide();
								psw[4]->hide();

								pb[2]->hide();
								pb[3]->hide();
								break;
							case 1:
								pe[4]->hide();
								pe[5]->hide();

								psw[1]->hide();
								psw[2]->hide();
								
								pb[0]->hide();
								pb[1]->hide();

								pe[6]->hide();
								pe[7]->hide();

								pl[0]->hide();

								pl[1]->show();
								pl[2]->show();
								pl[3]->show();

								psw[3]->show();
								psw[4]->show();

								pb[2]->show();
								pb[3]->show();
								break;
							}
							break;
						case 2:
							ptb[1]->hide();
							ptb[2]->hide();

							pe[2]->hide();
							pe[3]->hide();

							pe[4]->hide();
							pe[5]->hide();

							psw[1]->hide();
							psw[2]->hide();

							pb[0]->hide();
							pb[1]->hide();

							pe[6]->hide();
							pe[7]->hide();

							pl[0]->hide();

							pl[1]->hide();
							pl[2]->hide();
							pl[3]->hide();

							psw[3]->hide();
							psw[4]->hide();

							pb[2]->hide();
							pb[3]->hide();
							break;
						}
					}
				}
				break;
			case WM_DESTROY:
				{
					wchar_t c_s[256];
					wsprintf(c_s, L"%s.txt", c_name_c + 1);
					cai.ai.save_logic(f_save2(c_s));
				}
				SetEvent(h_destroy);
				{
					int i_num = sl.l_d_sk.number;
					int i = 0;
					for(socketlist::_data_sk *pd_sk = sl.l_d_sk.start(); pd_sk; pd_sk = sl.l_d_sk.next(pd_sk)){
						if(i_num == i)
							break;
						sc.Send(pd_sk->sk, "del ", 4);
						i++;
					}
				}
				break;
		}
	}
	void init(){
		ptr = multithread::getpointer(this);
		setpos(605, 0, 600 + 256, 600);
		SetTimer(hWnd, ID_TIMER1, 60000, NULL);
		ptb[3] = tab(0, 0, 256, 20);
		ptb[3]->ins(L"main", 8);
		//ptb[3]->ins(L"group", 9);
		//ptb[3]->set(0);
		pe[0] = edit(10, 256 + 40, 256 - 20, 600 - 80 - 256 - 15, 1000, true);
		pe[0]->setcolor(0, 0, 63, 255, 255, 255);
		pe[0]->set(L"");
		pe[1] = edit(10, 600 - 40, 256 - 20, 20, 1001);
		pe[1]->setcolor(0, 0, 63, 255, 255, 255);
		pe[1]->set(L"");

		pe[2] = edit(256 + 10, 10 + 40, 600 - 20, 300 - 20 - 20, 1002, NULL, false);
		pe[2]->setcolor(0, 0, 63, 255, 255, 255);
		pe[2]->set(L"");
		pe[3] = edit(256 + 10, 310 + 20, 600 - 20, 300 - 20 - 20, 1003, NULL, false);
		pe[3]->setcolor(0, 0, 63, 255, 255, 255);
		pe[3]->set(L"");

		ptb[0] = tab(256, 0, 600, 20);
		ptb[0]->ins(L"AI", 1);
		ptb[0]->ins(L"P2P", 2);
		//ptb[0]->ins(L"動画広場", 3);
		ptb[0]->set(0);


		ptb[1] = tab(256, 20, 600, 20);
		ptb[1]->ins(L"論理", 101);
		//ptb[1]->ins(L"神経細胞", 102);
		//ptb[1]->ins(L"動画生成", 103);
		
		ptb[2] = tab(256, 20, 600, 20);
		ptb[2]->ins(L"ﾈｯﾄ", 6);
		ptb[2]->ins(L"ﾀﾞｳﾝ", 7);
		ptb[2]->hide();
		pe[4] = edit(256 + 30, 50, 100, 15, 1004);
		pe[4]->setcolor(0, 0, 63, 255, 255, 255);
		pe[4]->set(L"");
		pe[4]->hide();
		pe[5] = edit(256 + 30 + 130, 50, 100, 15, 1005);
		pe[5]->setcolor(0, 0, 63, 255, 255, 255);
		pe[5]->set(L"");
		pe[5]->hide();
		psw[1] = staticwindow2(256, 50, 30, 15);
		psw[1]->phm->string(0, 0, L"IP:");
		psw[1]->hide();
		psw[2] = staticwindow2(256 + 130, 50, 30, 15);
		psw[2]->phm->string(0, 0, L"Port:");
		psw[2]->hide();
		pb[0] = colorbutton(L"server", 256 + 260, 50, 100, 20, 2000);
		pb[1] = colorbutton(L"client", 256 + 260 + 100, 50, 100, 20, 2001);
		pb[0]->hide();
		pb[1]->hide();
		pe[6] = edit(256 + 10, 200, 600 - 20, 370, 1006, true);
		pe[6]->setcolor(0, 0, 63, 255, 255, 255);
		pe[6]->set(L"");
		pe[7] = edit(256 + 10, 575, 600 - 20, 20, 1007);
		pe[7]->setcolor(0, 0, 63, 255, 255, 255);
		pe[7]->set(L"");
		pe[6]->hide();
		pe[7]->hide();
		pl[0] = list(256 + 10, 70, 600 - 20, 130, 3000);
		pl[0]->setcolor(0, 0, 63, 255, 255, 255);
		pl[0]->hide();

		pl[1] = list(256 + 10, 70, 600 - 20, 130, 3001);
		pl[1]->setcolor(0, 0, 63, 255, 255, 255);
		pl[1]->hide();
		psw[3] = staticwindow2(256 + 10, 220, 100, 20);
		psw[3]->phm->string(0, 0, L"Up:");
		psw[3]->hide();
		pl[2] = list(256 + 10, 240, 600 - 20, 130, 3002);
		pl[2]->setcolor(0, 0, 63, 255, 255, 255);
		pl[2]->hide();
		psw[4] = staticwindow2(256 + 10, 370, 100, 20);
		psw[4]->phm->string(0, 0, L"Down:");
		psw[4]->hide();
		pl[3] = list(256 + 10, 390, 600 - 20, 130, 3004);
		pl[3]->setcolor(0, 0, 63, 255, 255, 255);
		pl[3]->hide();
		pb[2] = colorbutton(L"download", 256 + 250, 200, 100, 20, 2002);
		pb[2]->hide();
		pb[3] = colorbutton(L"update", 256 + 250, 520, 100, 20, 2003);
		pb[3]->hide();

		print_up();
		print_down();

		pl[4] = list(0, 20, 256, 256, 3005);
		pl[4]->setcolor(0, 0, 63, 255, 255, 255);
		pl[4]->hide();
		psw[0] = staticwindow2(0, 20, 256, 256);
		wchar_t c_l[256];
		wsprintf(c_l, L"%s.bmp", c_name_c + 1);
		HBITMAP hbmp = (HBITMAP)LoadImage(NULL, c_l, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		HDC hMdc = CreateCompatibleDC(psw[0]->phm->h);
		SelectObject(hMdc, hbmp);
		BitBlt(psw[0]->phm->h, 0, 0, 256, 256, hMdc, 0, 0, SRCCOPY);
		DeleteDC(hMdc);
		DeleteObject(hbmp);
		SelectObject(hMdc, hbmp);
		psw[0]->invalidate();

		SetEvent(h_endofcreatewindow);
	}
};
struct _tr : public multithread::_thread{
	_tr(){
	}
	void func_analyze(ai3 *pa, conversation *pcv, int i_m2){
		//wchar_t c_buf4[256];
		//wchar_t c_out[256];	//pc_buf[0]
		int i = i_m2;
		while(1){
			i = (i + 1) % NUM_M;
			if(i == pa->i_m)
				break;
			wchar_t c_buf[65536];
			wchar_t c_buf2[65536];
			bool b = pa->get_out(c_buf, 65536, i);
			if(b){
				if(ws::wscmp(c_buf, L"#client{", 8)){
					int i2 = 8;
					wchar_t c_buf3[256];
					wchar_t c_buf4[256];

					for(int i = 0; i < 256; i++){
						if(c_buf[i2] == L':'){
							c_buf3[i] = 0;
							break;
						}
						c_buf3[i] = c_buf[i2];
						i2++;
					}
					c_buf3[255] = 0;

					i2++;
					for(int i = 0; i < 256; i++){
						if(c_buf[i2] == L'}'){
							c_buf4[i] = 0;
							break;
						}
						c_buf4[i] = c_buf[i2];
						i2++;
					}
					c_buf4[255] = 0;
					
					mt.begin(new _tr_client(c_buf3, c_buf4), L"client");
				}
				else if(ws::wscmp(c_buf, L"#server{", 8)){
					int i2 = 8;
					wchar_t c_buf3[256];

					for(int i = 0; i < 256; i++){
						if(c_buf[i2] == L'}'){
							c_buf3[i] = 0;
							break;
						}
						c_buf3[i] = c_buf[i2];
						i2++;
					}
					c_buf3[255] = 0;
					
					int i_port = _wtoi(c_buf3);

					mt.begin(new _tr_server(i_port), L"server");
				}
				else if(ws::wscmp(c_buf, L"#download{", 10)){
					int i2 = 10;
					wchar_t c_buf3[256];

					for(int i = 0; i < 256; i++){
						if(c_buf[i2] == L'}'){
							c_buf3[i] = 0;
							break;
						}
						c_buf3[i] = c_buf[i2];
						i2++;
					}
					c_buf3[255] = 0;
					
					filelist::_data *pd = fl.search(c_buf3);
					if(pd){
						pd->i_state = 1;
						fl.print(pl[1]);
						sc.Send(pd->s, "down", 4);
						int i_n = ws::wslen(pd->c_name) * 2 + 2;
						sc.Send(pd->s, (char *)&i_n, 4);
						sc.Send(pd->s, (char *)pd->c_name, i_n);
					}
				}
				else if(ws::wscmp(c_buf, L"#update", 7)){
					print_up();
					print_down();
					for(socketlist::_data_sk *pd_sk = sl.l_d_sk.start(); pd_sk; pd_sk = sl.l_d_sk.next(pd_sk)){
						//受信
						sc.Send(pd_sk->sk, "updt", 4);
						//送信
						struct _wfinddata_t fdata;
			
						int fh = _wfindfirst(L"./Up/*.*", &fdata);
						if(fh == -1){
						}
						else{
							do{
								if((fdata.attrib & _A_SUBDIR) == 0){
									sc.Send(pd_sk->sk, "fadd", 4);
									wchar_t c_name[1024];
									int i_len = ws::wslen(fdata.name) + 1;
									int i_len2 = i_len * 2;
									sc.Send(pd_sk->sk, (char *)&i_len2, 4);
									//mbstowcs(c_name, fdata.name, i_len);
									sc.Send(pd_sk->sk, (char *)fdata.name, i_len2);
								}
							}while(_wfindnext(fh, &fdata) == 0);
							//_findclose(fh);
						}
					}
				}
				else{
					wsprintf(c_buf2, L"%s：%s", c_name_c + 1, c_buf);
					pcv->set(c_buf2, pe[0]);
				}
			}
		}
	}
	void operator()(){
		WaitForSingleObject(h_endofcreatewindow, INFINITE);
		wchar_t c_l[256];
		wsprintf(c_l, L"%s.txt", c_name_c + 1);
		cai.ai.load_logic(f_load2(c_l));
		cai.ai.set_mem(ai3::_mem::M, L"#init");
		func_analyze(&cai.ai, &cvs, 0);
		cai.ai.print_mem(pe[2]);
		cai.ai.print_logic(pe[3]);
		while(1){
			if(!iszero()){
				int *pi = (int *)peek()->body.pc;
				switch(*pi){
					case TM_TIMER:
						{
							wchar_t c_buf6[256];
							wsprintf(c_buf6, L"#timer");
							int i_m2 = cai.ai.i_m;

							cai.ai.set_mem(ai3::_mem::T::I, c_buf6);

							func_analyze(&cai.ai, &cvs, i_m2);

							cai.ai.print_mem(pe[2]);
							cai.ai.print_logic(pe[3]);
						}
						break;
					case TM_UP:
						{
							char *pc = &peek()->body.pc[4];
							wchar_t c_buf6[512];
							wsprintf(c_buf6, L"#up{%s}", (wchar_t *)pc);
							int i_m2 = cai.ai.i_m;

							cai.ai.set_mem(ai3::_mem::T::I, c_buf6);

							func_analyze(&cai.ai, &cvs, i_m2);

							cai.ai.print_mem(pe[2]);
							cai.ai.print_logic(pe[3]);
						}
						break;
					case TM_CONNECT:
						{
							char *pc = &peek()->body.pc[4];
							wchar_t c_buf6[512];
							wsprintf(c_buf6, L"#connect{%s}", (wchar_t *)pc);
							int i_m2 = cai.ai.i_m;

							cai.ai.set_mem(ai3::_mem::T::I, c_buf6);

							func_analyze(&cai.ai, &cvs, i_m2);

							cai.ai.print_mem(pe[2]);
							cai.ai.print_logic(pe[3]);
						}
						break;
					case TM_FADD:
						{
							char *pc = &peek()->body.pc[4];
							wchar_t c_buf6[512];
							wsprintf(c_buf6, L"#net{%s}", (wchar_t *)pc);
							int i_m2 = cai.ai.i_m;

							cai.ai.set_mem(ai3::_mem::T::I, c_buf6);

							func_analyze(&cai.ai, &cvs, i_m2);

							cai.ai.print_mem(pe[2]);
							cai.ai.print_logic(pe[3]);
						}
						break;
					case TM_EDITRETURN:
						{
							char *pc = &peek()->body.pc[4];
							wchar_t c_buf[65536];
							wchar_t c_buf2[65536];
							pe[1]->get(c_buf, 65536);
							c_buf[65535] = 0;
							for(int i = 0; i < 65535; i++){
								if(c_buf[i] == 13 && c_buf[i + 1] == 10){
									c_buf[i] = 0;
									break;
								}
							}
							
							int i_m2 = cai.ai.i_m;
							cai.ai.set_mem(ai3::_mem::T::I, c_buf);

							wsprintf(c_buf2, L"あなた：%s", c_buf);
							cvs.set(c_buf2, pe[0]);

							func_analyze(&cai.ai, &cvs, i_m2);

							cai.ai.print_mem(pe[2]);
							cai.ai.print_logic(pe[3]);

						}
						break;
					case TM_EDITRETURN2:
						{
							char *pc = &peek()->body.pc[4];
							wchar_t c_buf[65536];
							wchar_t c_buf2[65536];
							pe[7]->get(c_buf, 65536);
							c_buf[65535] = 0;
							for(int i = 0; i < 65535; i++){
								if(c_buf[i] == 13 && c_buf[i + 1] == 10){
									c_buf[i] = 0;
									break;
								}
							}

							int i_cursel = pl[0]->getcursel();
							if(i_cursel >= 0){
								socketlist::_data_sk *pd_sk = sl.search_ar(i_cursel);
								conversation *pcv = pd_sk->pcv;

								wsprintf(c_buf2, L"あなた：%s", c_buf);
								pcv->set(c_buf2, pe[6]);

								int i_nd = ai3::wslen(c_buf) * 2 + 2;
								sc.Send(pd_sk->sk, "stc ", 4);
								sc.Send(pd_sk->sk, (char *)&i_nd, 4);
								sc.Send(pd_sk->sk, (char *)c_buf, i_nd);
							}
						}
						break;
				}
				pop();
			}
			else{
				wait(1000);
			}
		}
	}
};
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow){
	//i_state_net = 0;
	f_load2 fl(L"name.txt");
	fl.load_all((char *)c_name_c, 256 * 2);
	c_name_c[255] = 0;

	i_h_tr = 1;
	h_endofcreatewindow = CreateEvent(NULL, TRUE, FALSE, NULL);
	h_destroy = CreateEvent(NULL, TRUE, FALSE, NULL);
	mt.begin(new _proc(), L"window", 600 + 256, 600);
	mt.begin(new _tr(), L"thread");
	mt.printlist();
	//getch();
	WaitForSingleObject(h_destroy, -1);
}