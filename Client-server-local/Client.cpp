#include <iostream>
#include <string>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main() {

	string ipAddress = "127.0.0.1";  //Ip address of the server
	int port = 54000;				 //Listening port on the server

	//Initialize winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		cerr << "Can't start Winsock, Error: #" << wsResult << endl;
		return;
	}

	//Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "Can't ceeate socket, Error#" << WSAGetLastError() << endl;
		return;
	}
	//Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cerr << "Can't connect to server, Error #" << WSAGetLastError() << endl;
		closesocket(sock);
		return;
	}

	//Do-while loop to send and receive data
	
	char buf[4096];
	string userInput;

	do {
		//Prompt the user for some text
		cout << ">> ";
		getline(cin, userInput);

		
		if (userInput.size() > 0) {//Make sure the user has typed in something
			//Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {
				//Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0) {
					//Echo response to console
					cout << " SERVER>> " << string(buf, 0, bytesReceived) << endl;
				}
			}
		}
	} while (userInput.size() > 0);

	//Close everythig
	closesocket(sock);
	WSACleanup();
}