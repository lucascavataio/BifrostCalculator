#include "CalculatorForm.h"
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Windows::Forms;

int main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Creating the form instance using gcnew (since CalculatorForm is a ref class)
	BifrostCalculatorApp::CalculatorForm^ appUI = gcnew BifrostCalculatorApp::CalculatorForm();
	Application::Run(appUI);
	return 0;
}

namespace BifrostCalculatorApp {

	System::Void CalculatorForm::CalculatorButton_Click(System::Object^ sender, System::EventArgs^ e)
	{
		// Cast sender to a Button.
		Button^ btn = dynamic_cast<Button^>(sender);
		if (btn == nullptr)
			return;

		// If the text in the InputTextBox contains "nan" (case-insensitive), clear it.
		if (this->InputTextBox->Text->ToLower()->Contains("nan"))
			this->InputTextBox->Text = "";

		// Cache the current caret position.
		int cachedCaretPos = GetCaretPos();

		// Determine the string to insert.
		String^ textToInstert = btn->Text;

		// Handle special buttons.
		if (btn == this->SqrtButton)
			textToInstert = "sqrt";
		else if (btn == this->PowButton)
			textToInstert = "^";
		else if (btn == this->AnsButton)
		{
			if (String::IsNullOrEmpty(LastResult))
				return;
			textToInstert = LastResult;
		}

		// If the button's text is a math operator enters here
		if (Array::IndexOf(this->MathOperators, textToInstert->ToLower()) != -1) {

			//If the input text box is empty and we're about to insert an operator, 
			//the calculator automatically uses the last result before that operator. 
			//After all, if (something/0) equals infinity, just imagine what (void/something) would do...
			if (String::IsNullOrEmpty(this->InputTextBox->Text)) {
				if (String::IsNullOrEmpty(LastResult))
					textToInstert = textToInstert->Insert(0, "0");
				else
					textToInstert = textToInstert->Insert(0, LastResult);

				this->SetCaretPos(cachedCaretPos + textToInstert->Length, false);
			}
		}

		// If this is the Pi button, replace with the numeric value (as a string).
		bool isPiButton = (btn == this->PiButton);
		if (isPiButton)
			textToInstert = "3.141593";

		// If the button's (processed) text is one of the math functions, append parentheses.
		if (Array::IndexOf(this->MathFunctions, textToInstert->ToLower()) != -1)
		{
			textToInstert += "()";
			// Insert the text at the current caret position.
			this->InputTextBox->SelectedText = textToInstert;
			// Move the caret inside the parentheses.
			this->SetCaretPos(this->InputTextBox->SelectionStart - 1, false);
		}
		else
		{
			// For any other button, insert the text at the current caret position.
			this->InputTextBox->SelectedText = textToInstert;
			// Special case: if the text is exactly "()", move the caret inside.
			if (textToInstert == "()")
				this->SetCaretPos(this->InputTextBox->SelectionStart - 1, false);
		}

		// Adjust caret position for very short expressions or for the Pi button.
		if (this->InputTextBox->Text->Length > 0 && this->InputTextBox->Text->Length < 2)
		{
			this->SetCaretPos(1, false);
		}
		else if (isPiButton)
		{
			this->SetCaretPos(cachedCaretPos + textToInstert->Length, false);
		}

		// Ensure the TextBox remains focused.
		this->InputTextBox->Focus();
	}


	/// <summary>
	/// Implementation of the SendButton_Click event handler.
	/// </summary>
	System::Void CalculatorForm::SendButton_Click(System::Object^ sender, System::EventArgs^ e)
	{
		// Disabling the send button to prevent multiple clicks
		this->SendButton->Enabled = false;

		//System::Diagnostics::Debug::WriteLine("Sending expression to the microcontroller: " + this->GetCurrentExpression());

		// Getting the text from the InputTextBox.
		String^ expressionManaged = this->GetCurrentExpression()->Trim()->ToLower();

		// Converting the managed string to a native std::string.
		std::string expression = msclr::interop::marshal_as<std::string>(expressionManaged);

		// Creating an instance of your Bifrost class.
		Bifrost bridge;


		System::String^ managedCom = this->GetTargetCom();
		std::wstring targetCom = msclr::interop::marshal_as<std::wstring>(managedCom);

		// Opening the serial port.
		if (!bridge.Open(targetCom.c_str(), this->GetTargetBaudrate()))
		{
			MessageBox::Show("Failed to open serial port.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);

			this->SendButton->Enabled = true;
			return;
		}

		// Writing the expression to the serial port.
		if (!bridge.WriteData(expression + "\n"))
		{
			MessageBox::Show("Failed to write to serial port.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			bridge.Close();

			this->SendButton->Enabled = true;
			return;
		}

		std::string response = bridge.ReadData(200);
		bridge.Close();

		// Converting the native response back to a managed string.
		String^ responseManaged = gcnew String(response.c_str());

		//System::Diagnostics::Debug::WriteLine("Data recived from the microcontroller: " + responseManaged);

		if (responseManaged->ToLower()->Contains("nan")) {

			String^ msg = "SYNTAX ERROR: \nThe microcontroller couln't manage that expression. \n" + responseManaged->Replace("nan", "");
			MessageBox::Show(msg, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);

			this->SendButton->Enabled = true;
			return;
		}

		String^ finalResponse = responseManaged->Trim();

		try {
			// Try to convert the response to a double.
			double resultValue = Convert::ToDouble(finalResponse);

			// If the number is whole, format with no decimals.
			if (resultValue == Math::Floor(resultValue))
			{
				finalResponse = resultValue.ToString("F0");
			}
			else
			{
				// Otherwise, use a custom format that removes trailing zeros.
				finalResponse = resultValue.ToString("F6");
			}
		}
		catch (System::FormatException^) {
			// If conversion fails, finalResponse remains unchanged.
			// This handles cases like "ovf", "inf", or "nan".
		}

		this->LastResult = finalResponse;

		// Construct a full operation string.
		String^ fullOperation = this->GetCurrentExpression() + " = " + finalResponse;

		// Also add the operation to the ListBox for display at the top of the list, 
		// so it displays the operations from bottom to top.
		this->OperationsListBox->Items->Insert(0, fullOperation);

		//System::Diagnostics::Debug::WriteLine(this->OperationsListBox->Items->Count);

		this->InputTextBox->Text = "";

		this->SetCaretPos(this->InputTextBox->Text->Length, true);

		this->SendButton->Enabled = true;
	}


	System::Void CalculatorForm::DelButton_Click(System::Object^ sender, System::EventArgs^ e)
	{
		// If some text is selected, clear it.
		if (!System::String::IsNullOrEmpty(this->InputTextBox->SelectedText)) {
			this->InputTextBox->SelectedText = "";
		}
		else {
			// If nothing is selected, delete the character just before the caret.
			int caretPos = GetCaretPos();
			if (caretPos > 0 && this->InputTextBox->Text->Length > 0) {
				// Remove the character immediately before the caret.
				this->InputTextBox->Text = this->InputTextBox->Text->Remove(caretPos - 1, 1);
				// Move the caret to the correct position.
				this->InputTextBox->SelectionStart = caretPos - 1;
			}
		}

		this->InputTextBox->Focus();
	}

	/// <summary>
	/// Clears the whole calculator (Input text and List of previous operations).
	/// </summary>
	System::Void CalculatorForm::ClearInput()
	{
		this->InputTextBox->Text = "";

		this->OperationsListBox->Items->Clear();
	}

}