#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <stdio.h>
#include <string>

// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "40000" 
#define DEFAULT_BUFFER_LENGTH	512
void printByteArray(char* byData);
class Client {
public:
	Client(char* servername)
	{
		szServerName = servername;
		ConnectSocket = INVALID_SOCKET;
	}

	bool Start() {
		WSADATA wsaData;

		// Initialize Winsock
		int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if(iResult != 0)
		{
			printf("WSAStartup failed: %d\n", iResult);
			return false;
		}

		struct addrinfo	*result = NULL,
						*ptr = NULL,
						hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;		
		hints.ai_socktype = SOCK_STREAM;	
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		iResult = getaddrinfo(szServerName, DEFAULT_PORT, &hints, &result);
		if (iResult != 0)
		{
			printf("getaddrinfo failed: %d\n", iResult);
			WSACleanup();
			return false;
		}

		ptr = result;

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (ConnectSocket == INVALID_SOCKET)
		{
			printf("Error at socket(): %d\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return false;
		}

		// Connect to server
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
		}

		freeaddrinfo(result);

		if (ConnectSocket == INVALID_SOCKET)
		{
			printf("Unable to connect to server!\n");
			WSACleanup();
			return false;
		}

		return true;
	};

	// Free the resouces
	void Stop() {
		int iResult = shutdown(ConnectSocket, SD_SEND);

		if (iResult == SOCKET_ERROR)
		{
			printf("shutdown failed: %d\n", WSAGetLastError());
		}

		closesocket(ConnectSocket);
		WSACleanup();
	};

	// Send message to server
	bool Send(char* szMsg)
	{
		
		int iResult = send(ConnectSocket, szMsg, strlen(szMsg), 0);

		if (iResult == SOCKET_ERROR)
		{
			printf("send failed: %d\n", WSAGetLastError());
			Stop();
			return false;
		}

		return true;
	};

	// Receive message from server
	bool Recv()
	{
		char recvbuf[DEFAULT_BUFFER_LENGTH];
		int iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFFER_LENGTH, 0);

		if (iResult > 0)
		{
			char msg[DEFAULT_BUFFER_LENGTH];
			memset(&msg, 0, sizeof(msg));
			strncpy(msg, recvbuf, iResult);

			printf("\nReceived: %s\n", msg);

			return true;
		}


		return false;
	}

private:
	char* szServerName;
	SOCKET ConnectSocket;
};

void printByteArray(char* byData) {
	/*printf("0x");
	for (int i = 0; i < sizeof(byData); i++)
	{
		printf("%02X", byData[i]);
	}
	printf("\n");*/
	long code2 = (byData[1] & 0xFF)  | ((byData[0] & 0xFF) << 8);
	printf("\n0x%X", code2);
}

int main(int argc, CHAR* argv[]) 
{
	std::string msg;

	Client client("127.0.0.1");

	if (!client.Start())
		return 1;
		
	while(true)
	{
		std::cout << "Send: ";
		std::getline(std::cin, msg);

		// Close the connection when user enter "close"
		if (msg.compare("close") == 0)
		{
			break;
		}

		// convert
		char hexArray[256] = {0};
		int hexLength = 0;

		// read in the string
		unsigned int hex = 0;
		sscanf(msg.c_str(), "%x", &hex);
		
		// write it out
		for (unsigned int mask = 0xff000000, bitPos=24; mask; mask>>=8, bitPos-=8) {
			unsigned int currByte = hex & mask;
			if (currByte || hexLength) {
				hexArray[hexLength++] = currByte>>bitPos;
			}
		}
		printByteArray(hexArray);
		client.Send(hexArray);
		client.Recv();
	}

	client.Stop();

	getchar();
	return 0;
}