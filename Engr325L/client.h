/*
Client class for creating a socket connection with the server socket produced by a Raspberry Pi
Authors: Daniel and Micheal
Date: 2/12/2018
*/


#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

class Client {
public:
	Client();
	int transmit(int heading, int velocity);
	~Client();

private:
	SOCKET ConnectSocket;

};