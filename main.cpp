// Shotout to: Sloan Kelly; https://www.youtube.com/watch?v=0Zr_0Jy8mWE

#include<iostream>
#include<string>
#include<ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main() {
	string ipAddress = "Type the IP address here";			// IP address of the srver
	int port = 80;								// Listening port #(number) on the server

	// Initialize Winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		cerr << "Can't start winsock, Err #" << wsResult << endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Do-while loop to send and recive data
	char buf[4096];
	string userInput;

	do {
		// Prompt the user for some text
		cout << "> ";
		getline(cin, userInput);

		if (userInput.size() > 0) {
			// Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {
				// Wait for response
				ZeroMemory(buf, 4096);
				int byteReceived = recv(sock, buf, 4096, 0);
				if (byteReceived > 0) {
					//Echo response to console
					cout << "SERVER> " << string(buf, 0, byteReceived) << endl;
				}
			}
		}
	} while (userInput.size() > 0);

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();

}