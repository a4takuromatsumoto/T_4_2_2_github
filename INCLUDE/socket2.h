#ifndef MY_SOCKET
#define MY_SOCKET

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "wsock32.lib")

static unsigned count = 0;
static DWORD ipfromstring(char *ip){
	int b[4] = {0, 0, 0, 0};
	int p = 0;
	for(int ai = 0; ai < 4; ai++){
		while(ip[p] != '.' && ip[p] != 0x00){
			b[ai] *= 10;
			b[ai] += ip[p] - 0x30;
			p++;
		}
		p++;
	}
	DWORD d = 0;
	d = b[3] * 0x1000000 + b[2] * 0x10000 + b[1] * 0x100 + b[0];
	return d;
}
static DWORD ipfromstring(wchar_t *ip){
	int b[4] = {0, 0, 0, 0};
	int p = 0;
	for(int ai = 0; ai < 4; ai++){
		while(ip[p] != '.' && ip[p] != 0x00){
			b[ai] *= 10;
			b[ai] += ip[p] - L'0';
			p++;
		}
		p++;
	}
	DWORD d = 0;
	d = b[3] * 0x1000000 + b[2] * 0x10000 + b[1] * 0x100 + b[0];
	return d;
}
static void stringfromip(wchar_t *string, DWORD ip){
	int b[4];
	b[0] = ip & 0x000000ff;
	b[1] = (ip & 0x0000ff00) >> 8;
	b[2] = (ip & 0x00ff0000) >> 16;
	b[3] = (ip & 0xff000000) >> 24;
	wsprintf(string, L"%u.%u.%u.%u", b[0], b[1], b[2], b[3]);
}
static void stringfromip(char *string, DWORD ip){
	int b[4];
	b[0] = ip & 0x000000ff;
	b[1] = (ip & 0x0000ff00) >> 8;
	b[2] = (ip & 0x00ff0000) >> 16;
	b[3] = (ip & 0xff000000) >> 24;
	sprintf(string, "%u.%u.%u.%u", b[0], b[1], b[2], b[3]);
}
class socketclass
{
public:
	/*
	struct _clientdata{
		SOCKET s;
		in_addr in;
	};
	*/
	struct _clientdata{
		SOCKET s;
		union{
			DWORD d;
			in_addr in;
		};
	};


	WSADATA wsaData;
	socketclass()
	{
		if(count == 0){
			if(WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
				MessageBox(NULL, L"WSAStartup error", L"Error", MB_OK);
		}
		count++;
	}
	~socketclass()
	{
		count--;
		if(count == 0)
			WSACleanup();
	}
	SOCKET getsocket_client(char *addr, unsigned port){
		SOCKET s;
		LPHOSTENT lpHost;

		try{
			lpHost = gethostbyname(addr);//, strlen(addr), AF_INET);
			if(lpHost == NULL) throw L"gethostbyaddr error";
			s = Connect(lpHost, port);
		}
		catch(wchar_t *ec){
			MessageBox(NULL, ec, L"Error", MB_ICONEXCLAMATION);
			throw;
		}
		return s;
	}
	SOCKET noblock_client(char *addr, unsigned port, HWND hWnd, unsigned msg){
		SOCKET s;
		LPHOSTENT lpHost;
		//SOCKADDR_IN sockadd;

		try{
			lpHost = gethostbyname(addr);//, strlen(addr), AF_INET);
			if(lpHost == NULL) throw "gethostbyaddr error";
			s = noblock_Connect(lpHost, port, hWnd, msg);
		}
		catch(wchar_t *ec){
			MessageBox(NULL, ec, L"Error", MB_ICONEXCLAMATION);
			throw;
		}
		return s;
	}
	_clientdata getsocket_server(unsigned port){
		SOCKET s, listen_s;
		SOCKADDR_IN saddr, from;
		int fromlen;

		try{
			listen_s = socket(AF_INET, SOCK_STREAM, 0);
			if(listen_s < 0) throw "socket error";

			memset(&saddr, 0, sizeof(SOCKADDR_IN));
			saddr.sin_family = AF_INET;
			saddr.sin_port = htons(port);
			saddr.sin_addr.s_addr = INADDR_ANY;
			if(bind(listen_s, (struct sockaddr *)&saddr, sizeof(saddr)) == SOCKET_ERROR)
				throw L"bind error";

			if(listen(listen_s, 0) == SOCKET_ERROR)
				throw L"listen error";

			fromlen = (int)sizeof(from);
			s = accept(listen_s, (SOCKADDR *)&from, &fromlen);
			if(s == INVALID_SOCKET)
				throw L"accept error";
			Close(listen_s);
		}
		catch(wchar_t *ec){
			MessageBox(NULL, ec, L"Error", MB_ICONEXCLAMATION);
			throw;
		}
		_clientdata cd;
		cd.s = s;
		cd.in = from.sin_addr;
		return cd;
	}
	//上記を分割
	SOCKET getsocket_server1(){
		SOCKET listen_s;
		int fromlen;

		try{
			listen_s = socket(AF_INET, SOCK_STREAM, 0);
			if(listen_s < 0) throw L"socket error";


		}
		catch(wchar_t *ec){
			MessageBox(NULL, ec, L"Error", MB_ICONEXCLAMATION);
			throw;
		}
		return listen_s;
	}
	SOCKET getsocket_server2(SOCKET listen_s, unsigned port){
		SOCKET s;
		SOCKADDR_IN from;
		int fromlen;
		SOCKADDR_IN saddr;

		try{
			
			memset(&saddr, 0, sizeof(SOCKADDR_IN));
			saddr.sin_family = AF_INET;
			saddr.sin_port = htons(port);
			saddr.sin_addr.s_addr = INADDR_ANY;
			if(bind(listen_s, (struct sockaddr *)&saddr, sizeof(saddr)) == SOCKET_ERROR)
				throw L"bind error";

			if(listen(listen_s, 0) == SOCKET_ERROR)
				throw L"listen error";
			fromlen = (int)sizeof(from);
			s = accept(listen_s, (SOCKADDR *)&from, &fromlen);
			if(s == INVALID_SOCKET)
				throw L"accept error";
			Close(listen_s);
		}
		catch(wchar_t *ec){
			MessageBox(NULL, ec, L"Error", MB_ICONEXCLAMATION);
			throw;
		}
		return s;
	}
	void noblock_server(unsigned port, HWND hWnd, unsigned msg){
		SOCKET listen_s;
		SOCKADDR_IN saddr;
		int nAsync;

		try{
			listen_s = socket(AF_INET, SOCK_STREAM, 0);
			if(listen_s < 0) throw L"socket error";
			nAsync = WSAAsyncSelect(listen_s, hWnd, msg, FD_ACCEPT);
			if(nAsync != 0)
				throw L"WSAAsyncSelect(listen socket) error";
			memset(&saddr, 0, sizeof(SOCKADDR_IN));
			saddr.sin_family = AF_INET;
			saddr.sin_port = htons(port);
			saddr.sin_addr.s_addr = INADDR_ANY;
			if(bind(listen_s, (struct sockaddr *)&saddr, sizeof(saddr)) == SOCKET_ERROR)
				throw L"bind error";
			if(listen(listen_s, 0) == SOCKET_ERROR)
				throw "listen error";
		}
		catch(wchar_t *ec){
			wchar_t c[256];
			wsprintf(c, L"%s:%d", ec, WSAGetLastError());
			MessageBox(NULL, c, L"Error", MB_ICONEXCLAMATION);
			throw;
		}
	}
	_clientdata noblock_server_accept(SOCKET listen_s, HWND hWnd, unsigned msg){
		SOCKET s;
		SOCKADDR_IN from;
		int fromlen, nAsync;
		_clientdata cd;
		try{
			fromlen = (int)sizeof(from);
				s = accept(listen_s, (SOCKADDR *)&from, &fromlen);
				if(s == INVALID_SOCKET)
					throw L"accept error";
				nAsync = WSAAsyncSelect(s, hWnd, msg, FD_CLOSE | FD_READ);
				if(nAsync != 0)
					throw L"WSAAsyncSelect(socket) error";
		}
		catch(char *ec){
			wchar_t c[256];
			wsprintf(c, L"%s:%d", ec, WSAGetLastError());
			MessageBox(NULL, c, L"Error", MB_ICONEXCLAMATION);
			throw;
		}
		cd.s = s;
		cd.in = from.sin_addr;
		return cd;
	}
	unsigned Send(SOCKET s, char *p, unsigned size){
		return send(s, p, size, 0);
	}
	unsigned Recv(SOCKET s, char *p, unsigned size){
		unsigned pp = 0, nRtn;
		//printf("Recv呼び出されました(s = %d, p = %s, size = %d)\n", s, p, size);
		while(1)
		{
			//printf("pp = %d\n", pp);
			if(size < pp + 1024)
				nRtn = recv(s, p + pp, size - pp, 0);
			else
				nRtn = recv(s, p + pp, 1024, 0);
			//if(nRtn == 0)
			//	break;
			if(nRtn == SOCKET_ERROR)
			{
				int error = WSAGetLastError();
				throw L"socket_error\n";
				return 0xffffffff;
			}
			else
				pp += nRtn;
			if(pp == size)
				break;
		}
		//printf("無事にリターン(pp = %d)\n", pp);
		return pp;
	}
	unsigned Recv2(SOCKET s, char *p, unsigned size){
		unsigned pp = 0, nRtn;
		//printf("Recv呼び出されました(s = %d, p = %s, size = %d)\n", s, p, size);
		while(1)
		{
			//printf("pp = %d\n", pp);
			if(size < pp + 1024)
				nRtn = recv(s, p + pp, size - pp, 0);
			else
				nRtn = recv(s, p + pp, 1024, 0);
			//if(nRtn == 0)
			//	break;
			if(nRtn == SOCKET_ERROR)
			{
				int error = WSAGetLastError();
				throw L"socket_error\n";
				return 0xffffffff;
			}
			else if(nRtn == 0)
				break;
			else
				pp += nRtn;
			if(pp == size)
				break;
		}
		//printf("無事にリターン(pp = %d)\n", pp);
		return pp;
	}
	void Close(SOCKET s){
		closesocket(s);
	}
	SOCKET Connect(LPHOSTENT lpHost, unsigned port){
		SOCKET s;
		SOCKADDR_IN sockadd;

		s = socket(PF_INET, SOCK_STREAM, 0);
		if(s == INVALID_SOCKET) throw L"socket error";
		ZeroMemory(&sockadd, sizeof(sockadd));
		sockadd.sin_family = AF_INET;
		sockadd.sin_port = htons(port);
		sockadd.sin_addr = *((LPIN_ADDR)*lpHost->h_addr_list);
		if(connect(s, (PSOCKADDR)&sockadd, sizeof(sockadd)) != 0) throw L"connect error";
		return s;
	}
	SOCKET noblock_Connect(LPHOSTENT lpHost, unsigned port, HWND hWnd, unsigned msg){
		SOCKET s;
		SOCKADDR_IN sockadd;
		int nAsync;

		s = socket(PF_INET, SOCK_STREAM, 0);
		if(s == INVALID_SOCKET) throw L"socket error";
		nAsync = WSAAsyncSelect(s, hWnd, msg, FD_CLOSE | FD_READ | FD_CONNECT);
		if(nAsync != 0)
			throw L"WSAAsyncSelect(socket) error";
		ZeroMemory(&sockadd, sizeof(sockadd));
		sockadd.sin_family = AF_INET;
		sockadd.sin_port = htons(port);
		sockadd.sin_addr = *((LPIN_ADDR)*lpHost->h_addr_list);
		connect(s, (PSOCKADDR)&sockadd, sizeof(sockadd));
		return s;
	}
	static int big_to_little(int i){
		char *pc = (char *)&i;
		int i2;
		((char *)&i2)[0] = pc[3];
		((char *)&i2)[1] = pc[2];
		((char *)&i2)[2] = pc[1];
		((char *)&i2)[3] = pc[0];
		return i2;
	}
};
#endif