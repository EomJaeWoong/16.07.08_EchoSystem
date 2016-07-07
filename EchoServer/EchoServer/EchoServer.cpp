#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define BUF_SIZE 1000

void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main()
{
	WSADATA wsa;
	int retval;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	///////////////////////////////////////////////////////////////////////
	// listen socket 생성
	///////////////////////////////////////////////////////////////////////
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)	err_display("socket()");

	///////////////////////////////////////////////////////////////////////
	// bind
	///////////////////////////////////////////////////////////////////////
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9000);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));

	if (retval == SOCKET_ERROR)	err_display("bind()");

	///////////////////////////////////////////////////////////////////////
	// listen
	///////////////////////////////////////////////////////////////////////
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_display("listen()");
	printf("Server on..\n");

	///////////////////////////////////////////////////////////////////////
	// 데이터 통신에 필요한 변수
	///////////////////////////////////////////////////////////////////////
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUF_SIZE];

	while (1)
	{
		///////////////////////////////////////////////////////////////////////
		// accept
		///////////////////////////////////////////////////////////////////////
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			continue;
		}

		///////////////////////////////////////////////////////////////////////
		// translate...
		///////////////////////////////////////////////////////////////////////
		while (1)
		{
			recv(client_sock, buf, BUF_SIZE, 0);
			printf("Server : %s", buf);
		}
	}

	///////////////////////////////////////////////////////////////////////
	// close socket
	///////////////////////////////////////////////////////////////////////
	closesocket(client_sock);

	WSACleanup();

	return 0;
}