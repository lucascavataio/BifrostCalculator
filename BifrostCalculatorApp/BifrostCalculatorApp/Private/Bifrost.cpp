//Bifrost.cpp

#include <iostream>
#include <cstring> // For std::strlen

#include <windows.h>
#include "../public/Bifrost.h"

/// <summary>
/// Constructor for the Bifrost class. 
/// Initializes the serial handle to an invalid state.
/// </summary>
Bifrost::Bifrost()
{
    // Initialize your member variables.
    hSerial = INVALID_HANDLE_VALUE;
}

/// <summary>
/// Opens a serial communication port with the specified settings.
/// </summary>
/// <param name="port">The name of the COM port to open (e.g., L"COM4").</param>
/// <param name="baudrate">The baud rate for the communication (default is 9600).</param>
/// <returns>True if the port was successfully opened, false otherwise.</returns>
bool Bifrost::Open(LPCWSTR port, DWORD baudrate)
{
    hSerial = CreateFileW(port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        return false;
    }

    // Setting up the serial port parameters...

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        Close();
        return false;
    }

    dcbSerialParams.BaudRate = baudrate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        Close();
        return false;
    }

    // Set timeouts, etc.
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        Close();
        return false;
    }

    return true;
}

/// <summary>
/// Closes the serial communication port if it is open.
/// </summary>
void Bifrost::Close()
{
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}

/// <summary>
/// Writes data to the serial port.
/// </summary>
/// <param name="data">The string data to send through the serial port.</param>
/// <returns>True if the write operation was successful, false otherwise.</returns>
bool Bifrost::WriteData(const std::string& data) {
    if (hSerial == INVALID_HANDLE_VALUE)
        return false;

    DWORD bytesWritten;
    if (!WriteFile(hSerial, data.c_str(), data.size(), &bytesWritten, NULL)) {
        return false;
    }

    return true;
}

/// <summary>
/// Reads a specified number of bytes from the serial port.
/// </summary>
/// <param name="numBytes">The maximum number of bytes to read.</param>
/// <returns>A string containing the received data.</returns>
std::string Bifrost::ReadData(DWORD numBytes) {
    if (hSerial == INVALID_HANDLE_VALUE)
        return "";

    // Allocate a buffer to hold the incoming data.
    char* buffer = new char[numBytes + 1];
    DWORD bytesRead;
    if (!ReadFile(hSerial, buffer, numBytes, &bytesRead, NULL)) {
        delete[] buffer;
        return "";
    }
    buffer[bytesRead] = '\0';  // Null-terminate the C-string.
    std::string result(buffer);
    delete[] buffer;
    return result;
}
