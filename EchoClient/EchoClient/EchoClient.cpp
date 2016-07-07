#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define BUF_SIZE 30000
#pragma comment(lib, "winmm.lib") 

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

	///////////////////////////////////////////////////////////
	// socket
	///////////////////////////////////////////////////////////
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)		err_display("socket()");

	///////////////////////////////////////////////////////////
	// connect
	///////////////////////////////////////////////////////////
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9000);
	InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr);

	retval = connect(sock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)		err_display("connect()");

	char buf[BUF_SIZE];

	while (1)
	{
		memset(buf, 0, BUF_SIZE);

		for (int iCnt = 0; iCnt < rand() % BUF_SIZE - 1; iCnt++)
		{
			buf[iCnt] = (char)rand();
		}

		retval = send(sock, buf, strlen(buf), 0);
		printf("Send data..");
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
		Sleep(1000);
	}

	closesocket(sock);
	
	WSACleanup();

	return 0;
}