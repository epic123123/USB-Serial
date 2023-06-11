#include "Serial.h"

int main()
{
	UINT comPort = 0;
	ULONG BaudRate = 0;

	std::cout << "Enter com port number: ";
	std::cin >> comPort;

	std::cout << "Enter Baud rate: ";
	std::cin >> BaudRate;

	SerialPort serial(comPort, BaudRate);

	if (serial.hasFailed())
	{
		return 1;
	}

	while (serial.isConnected())
	{
		char buffer[INPUT_DATA_BYTES];
		serial.ReadCOM(buffer, INPUT_DATA_BYTES);

		std::string input(buffer);

		input.erase(input.find('\r'));

		std::cout << input << std::endl;

		Sleep(ARDUINO_PRINT_INTERVAL);
	}

	return 0;
}