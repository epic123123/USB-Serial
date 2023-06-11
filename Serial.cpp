

#include "Serial.h"

SerialPort::SerialPort(UINT comPort, ULONG BaudRate)
{
    this->connected = false;
    this->errors = 0;
    this->status = { 0 };
    this->failed = false;

    this->hComm = OpenCommPort((ULONG)comPort, GENERIC_READ, NULL);

    DWORD errCode = GetLastError();

    if (hComm == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to open COM" << comPort << "\nError code: " << errCode << "\n";
        failed = true;
    }

    switch (errCode)
    {
    case 2: {
        std::cout << "Reason: Port not found\n";
        return;
        break;
    }
    case 5: {
        std::cout << "Reason: another app is using port";
        return;
        break;
    }
    case 0: {
        std::cout << "Port opened successfully\n";
        break;
    }
    default:
    {
        std::cout << "Reason: Unknown error\n";
        return;
        break;
    }
    }

    DCB dcbSerialParameters = { 0 };

    if (!GetCommState(this->hComm, &dcbSerialParameters))
    {
        std::cout << "Failed to get serial parameters, error code: " << GetLastError();
        failed = true;
        return;
    }

    dcbSerialParameters.BaudRate = BaudRate;
    dcbSerialParameters.ByteSize = 8;
    dcbSerialParameters.StopBits = ONESTOPBIT;
    dcbSerialParameters.Parity = NOPARITY;
    dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

    if (!SetCommState(hComm, &dcbSerialParameters))
    {
        std::cout << "Failed to set comm state, error code: " << GetLastError();
        failed = true;
        return;
    }

    this->connected = true;
    std::cout << "Connected to com port succesfully!\n";

    PurgeComm(this->hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
    Sleep(2000);

}

SerialPort::~SerialPort()
{
    if (connected)
    {
        connected = false;
        CloseHandle(this->hComm);
        std::cout << "Connection closed\n";
    }
}


int SerialPort::ReadCOM(char* buffer, unsigned int buf_size)
{
    DWORD bytesRead = 0;
    unsigned int toRead = 0;

    ClearCommError(this->hComm, &this->errors, &this->status);

    if (status.cbInQue > buf_size)
    {
        toRead = buf_size;
    }
    else if (status.cbInQue > 0)
    {
        toRead = status.cbInQue;
    }

    if (ReadFile(hComm, buffer, toRead, &bytesRead, NULL))
    {
        return bytesRead;
    } 
    return 0;
}



bool SerialPort::isConnected()
{
    return this->connected;
}

bool SerialPort::hasFailed()
{
    return this->failed;
}