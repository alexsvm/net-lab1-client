#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// � ������ ������� ���������� � Additional Dependancies ���������� Ws2_32.lib

int __cdecl main(void)
{
	WSADATA wsaData;
	SOCKET ConnectSocket;  // ���������� ����� � ����� ��� ��������
	sockaddr_in ServerAddr;  // ��� ����� ����� �������
	int err, maxlen = 512;  // ��� ������ � ������ �������
	char* recvbuf = new char[maxlen];  // ����� ������
	char* query = new char[maxlen];  // ����� ��������
	char* server_ip = (char*)malloc(16); // ����� ��� ip-������
	int server_port; // ip-�����

    // Initialize Winsock
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	printf("Enter server ip:");
	scanf("%16s", server_ip);
	printf("Enter server port:");
	
	scanf("%d", &server_port);
	// Connect to server
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(server_ip);
	ServerAddr.sin_port = htons(server_port);
	
	err = connect(ConnectSocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr));

	if (err == SOCKET_ERROR) {
		printf("connect failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	while (true) {
		printf(">");
		gets_s(query, 250);

		if (strcmp(query, "quit") == 0)
			break;
		// �������� ������:
		send(ConnectSocket, query, strlen(query), 0);
		// ���� �����:
		err = recv(ConnectSocket, recvbuf, maxlen, 0);
		if (err > 0) {
			recvbuf[err] = 0;
			printf("Result: %s", (char*)recvbuf);
		}
		else if (err == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}
	}

	// shutdown the connection since we're done
	closesocket(ConnectSocket);
	printf("Press Enter to quit...\n");
	getchar();

}