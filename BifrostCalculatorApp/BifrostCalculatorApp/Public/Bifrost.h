#include <windows.h>
#include <string>

class Bifrost {
public:
    Bifrost();

    // Opens the serial port. 
    // portName should be something like L"\\\\.\\COM4" (recommended format for Windows).
    // The baudRate defaults to CBR_9600.
    bool Open(LPCWSTR portName, DWORD baudRate = CBR_9600);

    // Closes the serial port if it is open.
    void Close();

    // Writes data to the serial port.
    // Returns true if the write is successful.
    bool WriteData(const std::string &data);

    // Reads up to numBytes from the serial port.
    // Returns the data read as a std::string.
    std::string ReadData(DWORD numBytes);

private:
    HANDLE hSerial;  // Handle for the serial port.
};
