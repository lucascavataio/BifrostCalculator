# Bifrost Calculator - Project Structure

## Introduction

The **Bifrost Calculator** is a Windows desktop app that talks to a microcontroller through **serial communication** to solve mathematical expressions. This document gives an overview of how the software is structured, along with instructions to build, run, and test it.

<br>

## Index
- [Introduction](#introduction)
- [Software Architecture Overview](#software-architecture-overview)
- [Project Structure](#project-structure)
- [Key Components](#key-components)
  - [Windows Desktop Application](#windows-desktop-application)
  - [Microcontroller Firmware](#microcontroller-firmware)
- [Compiling and Running the Software](#compiling-and-running-the-software)
  - [Compiling and Running the Windows Application](#compiling-and-running-the-windows-application)
  - [Uploading the Sketch to the Microcontroller](#uploading-the-sketch-to-the-microcontroller)
  - [Testing the Connection](#testing-the-connection)
- [Code Documentation](#code-documentation)

<br>

## **Software Architecture Overview**

The project is made up of **two main parts**:
1. **Windows Desktop Application** (C++/CLI with Windows Forms)
2. **Microcontroller Firmware** (C++ for Arduino)

The **Windows app** sends math expressions over the serial port to the **microcontroller**, which processes them and sends back the result.

---

## **Project Structure**

<br>

<p align="center">
  <img src="https://github.com/lucascavataio/BifrostCalculator/blob/main/Documentation/src/ProjectStructure.png" 
       width="600" 
       style="border-radius: 10px; box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.2);">
  <br>
</p>

<br>

---

<br>

## **Key Components**

### **Windows Desktop Application**

#### **CalculatorForm.h / CalculatorForm.cpp**
- **Handles the user interface** using Windows Forms.
- Manages user input and **sends expressions to the microcontroller**.
- Displays the results once received from the microcontroller.
- **History feature:** Lets users click past results to reuse them.
- Uses the **Bifrost class** for serial communication.

#### **Bifrost.h / Bifrost.cpp**
- Manages **serial communication** between the PC and microcontroller.
- Includes functions to:
  - **Open and close the serial port**
  - **Send expressions to the microcontroller**
  - **Receive the computed result**

### **Microcontroller Firmware**

#### **BifrostCalculator.ino**
- Runs on the microcontroller (Arduino/ESP32).
- Uses **TinyExpr** to evaluate math expressions.
- Sends the computed result back to the PC over **UART (serial communication)**.

#### **TinyExpr Library**
- A lightweight math parser.
- Handles expressions like `sin(1.57) + sqrt(9)` without complex manual coding.

<br>

---

<br>

## **Compiling and Running the Software**

### **Compiling and Running the Windows Application**
#### **Requirements:**
- **Visual Studio**
- **Windows Forms project support**
- **C++/CLI tools installed:**

<br>

<p align="center">
  <img src="https://github.com/lucascavataio/BifrostCalculator/blob/main/Documentation/src/CLR.png" 
       width="600" 
       style="border-radius: 10px; box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.2);">
  <br>
</p>

<br>

#### **Steps:**
1. Open `BifrostCalculator.sln` in **Visual Studio**.
2. Click **Build → Build Solution**.
3. Click **Start** to run the application.

### **Uploading the Sketch to the Microcontroller**
#### **Requirements:**
- **Arduino IDE**
- **Microcontroller (Arduino/ESP32)**

#### **Steps:**
1. Install the **TinyExpr library** (Installation steps in `ArduinoGuide.pdf`).
2. Open `BifrostCalculator.ino` in **Arduino IDE**.
3. Select the correct **board and port**.
4. Click **Upload**.

<br>

---

<br>

### **Testing the Connection**
#### **Steps:**
1. Open the **Bifrost Calculator** application.
2. Enter the correct **COM port and baud rate**.
3. Type a **math expression** (e.g., `5+3*2`) and press **Enter**.
4. The microcontroller processes the expression and sends the result back.

> :warning: **Make sure the Serial Monitor in Arduino IDE is closed** before using the Windows app, otherwise the port will be **busy**.

<br>

---

<br>

# Bifrost Calculator - Arduino Setup Guide

<br>

## Introduction

Alright, so you’re ready to get the **Bifrost Calculator** running on your microcontroller? This guide will walk you through **installing the required library (TinyExpr)** and **uploading the sketch to your board**. Let’s get started!

---

## **Installing the TinyExpr Library**
Before uploading the sketch, we need to install **TinyExpr**, a lightweight math expression parser used in this project.

### **1. Downloading TinyExpr**
- Head over to the **TinyExpr GitHub Repository**: 
  > <a href="https://github.com/codeplea/tinyexpr" style="color: #66ccff; text-decoration: none; font-weight: bold;">TinyExpr GitHub Repo</a>
- Click **“Download ZIP”** to get the latest version of the library.

Or you can use the **TinyExpr** library that is already downloaded in `'BifrostCalculator\ArduinoSketches\Libraries'`.

### **2. Adding TinyExpr to Arduino IDE**
- Open **Arduino IDE**.
- Navigate to **Sketch** → **Include Library** → **Add .ZIP Library…**.

<p align="center">
  <img src="https://github.com/lucascavataio/BifrostCalculator/blob/main/Documentation/src/AddLibrary.png" 
       width="600" 
       alt="Adding Tinyexpr Library"
       style="border-radius: 10px; box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.2);">
  <br>
</p>

<br>

---

<br>

- Select the **TinyExpr zip** and click **Open**.

<br>

<p align="center">
  <img src="https://github.com/lucascavataio/BifrostCalculator/blob/main/Documentation/src/AddZip.png" 
       width="600" 
       alt="Adding Tinyexpr Zip"
       style="border-radius: 10px; box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.2);">
  <br>
</p>

<br>

- The library should now be installed.

To verify, go to **Sketch** → **Include Library** and check if **TinyExpr** is listed.

<br>

---

<br>

## **Uploading the Sketch to the Microcontroller**
Now that the TinyExpr library is installed, let’s upload the calculator sketch.

<br>

<p align="center">
  <img src="https://github.com/lucascavataio/BifrostCalculator/blob/main/Documentation/src/Upload.png" 
       width="600" 
       alt="Uploading Sketch"
       style="border-radius: 10px; box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.2);">
  <br>
</p>

<br>

---

<br>

The microcontroller is now running the Bifrost Calculator firmware.

<br>

## **Testing the Setup**
To ensure everything works as expected:
1. Open the **Serial Monitor** in Arduino IDE (**Tools** → **Serial Monitor**).
2. Set the **baud rate** to **9600** (or the rate you configured).
3. Type a **math expression** (e.g., `5+3*2`) and press **Enter**.
4. If everything is working, the microcontroller will **evaluate the expression** and return the result.

### **Important Note**
If the **Serial Monitor is open in Arduino IDE**, the **Bifrost Calculator will not be able to connect to the microcontroller**. The serial port can only be used by one application at a time, so close the **Serial Monitor** before trying to use the calculator.

<br>

<p align="center">
  <img src="https://github.com/lucascavataio/BifrostCalculator/blob/main/Documentation/src/PortErrorSerialMonitor.png" 
       width="600" 
       alt="Uploading Sketch"
       style="border-radius: 10px; box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.2);">
  <br>
</p>

<br>

<br>

---

<br>

You’ve successfully **installed TinyExpr, uploaded the sketch, and tested the setup**. Now you’re ready to use the **Bifrost Calculator** with your Windows application.

If you encounter issues, check your board’s COM port settings, make sure TinyExpr is installed correctly, and try restarting Arduino IDE.

<br>

---

<br>

## Code Documentation

Below is a brief overview of the key files you provided and some of the important functions contained within. This section will help maintainers and future developers quickly understand each file's role and its primary functions.

### **Bifrost.h**
- **Class `Bifrost`:** Manages serial communication from the Windows application to the microcontroller.
  - **`Open(LPCWSTR portName, DWORD baudRate)`:** Opens the specified COM port and configures its parameters (baud rate, etc.).  
  - **`Close()`:** Closes the COM port if it is open.  
  - **`WriteData(const std::string &data)`:** Sends string data through the open serial port.  
  - **`ReadData(DWORD numBytes)`:** Reads up to `numBytes` from the serial port and returns it as a `std::string`.

### **Bifrost.cpp**
- **Implements the `Bifrost` class** declared in `Bifrost.h`.  
  - **Constructor `Bifrost::Bifrost()`:** Initializes the serial handle to an invalid state (`INVALID_HANDLE_VALUE`).  
  - **`bool Bifrost::Open(...)`:** Uses the Windows API to open and configure the serial port. If initialization fails, it cleans up and returns `false`.  
  - **`void Bifrost::Close()`:** Safely closes the handle with `CloseHandle()` if valid.  
  - **`bool Bifrost::WriteData(...)`:** Writes the provided string to the port; returns success status.  
  - **`std::string Bifrost::ReadData(...)`:** Allocates a buffer to read from the port, reads up to `numBytes`, null-terminates, and returns it.

### **CalculatorForm.h**
- **`CalculatorForm` class:** A Windows Forms application (C++/CLI) that acts as the GUI for the calculator.  
  - **Properties like `LastResult`, `MathFunctions`, `MathOperators`:** Store the last calculation result and accepted functions/operators.  
  - **UI Handling Methods:** 
    - **`CalculatorForm()` constructor**: Initializes the form, sets up event handlers, and configures default UI settings.  
    - **`GetCurrentExpression()`, `GetCaretPos()`, `SetCaretPos(...)`:** Manage text input and caret position within the calculator's text box.  
    - **`ClearInput()`:** Clears the current input and the list of past operations.  
    - **`GetTargetCom()`, `GetTargetBaudrate()`:** Retrieve the COM port and baud rate from the form for connecting to the microcontroller.

### **CalculatorForm.cpp**
- **Implements the methods declared in `CalculatorForm.h`.**  
  - **`int main(array<String^>^ args)`:** Initializes and runs the Windows Forms application.  
  - **`CalculatorForm::CalculatorButton_Click(...)`:** Handles button inputs (e.g., digits, operators, special functions) and updates the input box.  
  - **`CalculatorForm::SendButton_Click(...)`:** The core routine for sending the expression to the microcontroller:  
    1. Opens the selected COM port (via `Bifrost::Open`).  
    2. Writes the expression.  
    3. Reads back the result.  
    4. Closes the port and updates the display with the returned value.  
  - **`OperationsListBox` event handlers**: Let users select old results to auto-fill the input field.
