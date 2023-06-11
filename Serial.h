#pragma once

#define ARDUINO_PRINT_INTERVAL 500
#define INPUT_DATA_BYTES 7

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#pragma comment(lib, "OneCore.lib")

class SerialPort
{
private:
	HANDLE hComm;
	bool connected;
	COMSTAT status;
	DWORD errors;
	bool failed; // if failed is set, then the current object should not be used.

public:
	SerialPort(UINT comPort, ULONG BaudRate);
	~SerialPort();

	int ReadCOM(char* buffer, unsigned int buf_size);
	bool isConnected();
	bool hasFailed();
};
