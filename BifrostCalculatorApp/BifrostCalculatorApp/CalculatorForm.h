#pragma once

#include <msclr/marshal_cppstd.h>
#include "./Public/Bifrost.h"

namespace BifrostCalculatorApp {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// CalculatorForm class for the Bifrost calculator application.
	/// Handles UI interaction and communication with the microcontroller.
	/// </summary>
	public ref class CalculatorForm : public System::Windows::Forms::Form
	{

	public:
		/// <summary>
		/// Array of supported mathematical functions.
		/// </summary>
		array<String^>^ MathFunctions;
	public:
		/// <summary>
		/// Array of supported mathematical operators.
		/// </summary>
		array<String^>^ MathOperators;

	public:
		String^ LastResult;

	public:
		/// <summary>
		/// Gets the target COM port from the UI input.
		/// </summary>
		/// <returns>COM port as a String.</returns>
		String^ GetTargetCom() {
			return this->ComTextBox->Text;
		}

	public:
		/// <summary>
		/// Gets the target baud rate from the UI input.
		/// Returns default CBR_9600 if parsing fails.
		/// </summary>
		/// <returns>Baud rate as an integer.</returns>
		int GetTargetBaudrate() {
			int baud;
			if (Int32::TryParse(this->BaudRateTextBox->Text, baud)) {
				return baud;
			}
			else {
				return CBR_9600;
			}
		}

	public:
		/// <summary>
		/// Gets the current caret position in the input textbox.
		/// </summary>
		/// <returns>Selection start index.</returns>
		int GetCaretPos() {
			return this->InputTextBox->SelectionStart;
		}

	public:
		/// <summary>
		/// Constructor for CalculatorForm.
		/// Initializes UI components and sets up event handlers.
		/// </summary>
		CalculatorForm(void)
		{
			InitializeComponent();
			AddButtonEventHandlers();

			MathFunctions = gcnew array<String^>{ "sin", "cos", "tan", "log", "ln", "sqrt", "pow" };
			MathOperators = gcnew array<String^>{ "+", "-", "/", "*", "%", "^" };

			LastResult = "";

			this->InputTextBox->HideSelection = false;
		}

	protected:
		/// <summary>
		/// Destructor to clean up resources.
		/// </summary>
		~CalculatorForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Clears the input text box when the form loads.
		/// </summary>
		System::Void CalculatorForm_Load(System::Object^ sender, System::EventArgs^ e) {
			this->InputTextBox->Text = "";
		}

	public:
		/// <summary>
		/// Sets the caret position in the input textbox.
		/// </summary>
		/// <param name="newIndex">New caret position.</param>
		/// <param name="focus">Whether to set focus on the textbox.</param>
		System::Void SetCaretPos(int newIndex, bool focus) {
			if (newIndex < 0) {
				System::Diagnostics::Debug::WriteLine("Caret less than 0: " + newIndex);
				return;
			}
			this->InputTextBox->SelectionStart = newIndex;
			if (focus) this->InputTextBox->Focus();
		}

	public:
		/// <summary>
		/// Gets the current expression from the input textbox.
		/// </summary>
		/// <returns>Expression as a String.</returns>
		System::String^ GetCurrentExpression() {
			return this->InputTextBox->Text;
		}

#pragma region OperationsListBox

	private:
		System::Void CalculatorForm::OperationsListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
		{
			// Check if an item is selected.
			if (this->OperationsListBox->SelectedItem == nullptr)
				return;

			// Get the full operation string.
			String^ operation = this->OperationsListBox->SelectedItem->ToString();

			// Find the index of the "=" character.
			int equalIndex = operation->IndexOf("=");
			if (equalIndex != -1 && equalIndex < operation->Length - 1)
			{
				// Extract the substring after "=" and trim any whitespace.
				String^ resultPart = operation->Substring(equalIndex + 1)->Trim();

				// Update the InputTextBox with the result.
				this->InputTextBox->SelectedText = resultPart;
			}
		}

	private:
		System::Void CalculatorForm::OperationsListBox_DrawItem(System::Object^ sender, System::Windows::Forms::DrawItemEventArgs^ e)
		{
			// Ensure there is a valid index.
			if (e->Index < 0)
				return;

			// Get the text of the item.
			String^ itemText = this->OperationsListBox->Items[e->Index]->ToString();

			// Draw the background (important for selection highlighting)
			e->DrawBackground();

			// Create a brush for the text.
			Brush^ textBrush = gcnew SolidBrush(e->ForeColor);

			// Horizontal margin (5 pixels)
			float offsetX = 5;
			// Calculate vertical offset to center the text in the available item height.
			float offsetY = (e->Bounds.Height - e->Font->Height) / 2;

			// Draw the string with the calculated offsets.
			e->Graphics->DrawString(itemText, e->Font, textBrush, e->Bounds.X + offsetX, e->Bounds.Y + offsetY);

			// Draw focus rectangle if the item is selected.
			e->DrawFocusRectangle();
		}


#pragma endregion

#pragma region Buttons Logic

#pragma region Buttons Event Handlers

	private:
		/// <summary>
		/// Adds event handlers to buttons in the calculator UI.
		/// </summary>
		void AddButtonEventHandlers()
		{
			//Numbers
			this->ZeroButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->OneButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->TwoButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->ThreeButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->FourButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->FiveButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->SixButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->SevenButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->EightButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->NineButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);

			this->PiButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->DotButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);

			//Functions
			this->SinButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->CosButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->TanButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->LogButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->LnButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->PowButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->SqrtButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);

			//Operators
			this->DivideButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->MultiplyButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->AddButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->SubstractButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->ModulusButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);

			//Sender
			this->SendButton->Click += gcnew System::EventHandler(this, &CalculatorForm::SendButton_Click);

			//Control
			this->ClearButton->Click += gcnew System::EventHandler(this, &CalculatorForm::ClearButton_Click);
			this->DelButton->Click += gcnew System::EventHandler(this, &CalculatorForm::DelButton_Click);
			this->AnsButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->ParenthesisButton->Click += gcnew System::EventHandler(this, &CalculatorForm::CalculatorButton_Click);
			this->OperationsListBox->DrawItem += gcnew System::Windows::Forms::DrawItemEventHandler(this, &CalculatorForm::OperationsListBox_DrawItem);

			this->OperationsListBox->SelectedIndexChanged +=
				gcnew System::EventHandler(this, &CalculatorForm::OperationsListBox_SelectedIndexChanged);
		}

#pragma endregion

	private:
		/// <summary>
		/// Handles click events for operator buttons.
		/// </summary>
		System::Void CalculatorButton_Click(System::Object^ sender, System::EventArgs^ e);

	private:
		/// <summary>
		/// Handles click events for the Send button.
		/// </summary>
		System::Void SendButton_Click(System::Object^ sender, System::EventArgs^ e);

	private:
		/// <summary>
		/// Handles click events for the Clear button.
		/// </summary>
		System::Void ClearButton_Click(System::Object^ sender, System::EventArgs^ e) {
			ClearInput();
		}

	private:
		/// <summary>
		/// Handles click events for the Delete button.
		/// </summary>
		System::Void DelButton_Click(System::Object^ sender, System::EventArgs^ e);

	private:
		/// <summary>
		/// Clears the input field.
		/// </summary>
		System::Void ClearInput();

#pragma endregion

		// All code BELOW is part of the automatic code builder in the form designer

#pragma region UI Components

	private: System::Windows::Forms::Label^ AppNameLabel;
	private: System::Windows::Forms::Label^ AppSubLabel;

	private:

		System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel5;
		System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel6;
	private: System::Windows::Forms::TableLayoutPanel^ ButtonsContainer;
	private: System::Windows::Forms::Button^ ParenthesisButton;
	private: System::Windows::Forms::Button^ ModulusButton;
	private: System::Windows::Forms::Button^ PowButton;
	private: System::Windows::Forms::Button^ SqrtButton;
	private: System::Windows::Forms::Button^ ZeroButton;
	private: System::Windows::Forms::Button^ OneButton;
	private: System::Windows::Forms::Button^ DotButton;
	private: System::Windows::Forms::Button^ TwoButton;
	private: System::Windows::Forms::Button^ SendButton;
	private: System::Windows::Forms::Button^ ThreeButton;
	private: System::Windows::Forms::Button^ SubstractButton;
	private: System::Windows::Forms::Button^ AddButton;
	private: System::Windows::Forms::Button^ MultiplyButton;
	private: System::Windows::Forms::Button^ SixButton;
	private: System::Windows::Forms::Button^ FiveButton;
	private: System::Windows::Forms::Button^ FourButton;
	private: System::Windows::Forms::Button^ SevenButton;
	private: System::Windows::Forms::Button^ EightButton;
	private: System::Windows::Forms::Button^ NineButton;
	private: System::Windows::Forms::Button^ SinButton;
	private: System::Windows::Forms::Button^ CosButton;
	private: System::Windows::Forms::Button^ TanButton;
	private: System::Windows::Forms::Button^ DivideButton;
	private: System::Windows::Forms::Button^ DelButton;
	private: System::Windows::Forms::Button^ PiButton;
	private: System::Windows::Forms::Button^ LogButton;
	private: System::Windows::Forms::Button^ ClearButton;
	private: System::Windows::Forms::Button^ LnButton;
	private: System::Windows::Forms::Panel^ TextFieldPanel;
	private: System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel1;
	private: System::Windows::Forms::Button^ AnsButton;
	private: System::Windows::Forms::TextBox^ InputTextBox;
	private: System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel7;
	private: System::Windows::Forms::ListBox^ OperationsListBox;

	private: System::Windows::Forms::Label^ AngleLabel;

	private: System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel2;
	private: System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel3;
	private: System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel4;
	private: System::Windows::Forms::TextBox^ BaudRateTextBox;

	private: System::Windows::Forms::TextBox^ ComTextBox;
	private: System::Windows::Forms::Label^ PortLabel;


	private: System::Windows::Forms::Label^ BaudLabel;




#pragma endregion

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(CalculatorForm::typeid));
			this->AppNameLabel = (gcnew System::Windows::Forms::Label());
			this->AppSubLabel = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel2 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tableLayoutPanel4 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->BaudLabel = (gcnew System::Windows::Forms::Label());
			this->BaudRateTextBox = (gcnew System::Windows::Forms::TextBox());
			this->tableLayoutPanel3 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->ComTextBox = (gcnew System::Windows::Forms::TextBox());
			this->PortLabel = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel5 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tableLayoutPanel6 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->ButtonsContainer = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->ParenthesisButton = (gcnew System::Windows::Forms::Button());
			this->ModulusButton = (gcnew System::Windows::Forms::Button());
			this->PowButton = (gcnew System::Windows::Forms::Button());
			this->SqrtButton = (gcnew System::Windows::Forms::Button());
			this->ZeroButton = (gcnew System::Windows::Forms::Button());
			this->OneButton = (gcnew System::Windows::Forms::Button());
			this->DotButton = (gcnew System::Windows::Forms::Button());
			this->TwoButton = (gcnew System::Windows::Forms::Button());
			this->SendButton = (gcnew System::Windows::Forms::Button());
			this->ThreeButton = (gcnew System::Windows::Forms::Button());
			this->SubstractButton = (gcnew System::Windows::Forms::Button());
			this->AddButton = (gcnew System::Windows::Forms::Button());
			this->MultiplyButton = (gcnew System::Windows::Forms::Button());
			this->SixButton = (gcnew System::Windows::Forms::Button());
			this->FiveButton = (gcnew System::Windows::Forms::Button());
			this->FourButton = (gcnew System::Windows::Forms::Button());
			this->SevenButton = (gcnew System::Windows::Forms::Button());
			this->EightButton = (gcnew System::Windows::Forms::Button());
			this->NineButton = (gcnew System::Windows::Forms::Button());
			this->SinButton = (gcnew System::Windows::Forms::Button());
			this->CosButton = (gcnew System::Windows::Forms::Button());
			this->TanButton = (gcnew System::Windows::Forms::Button());
			this->DivideButton = (gcnew System::Windows::Forms::Button());
			this->DelButton = (gcnew System::Windows::Forms::Button());
			this->PiButton = (gcnew System::Windows::Forms::Button());
			this->LogButton = (gcnew System::Windows::Forms::Button());
			this->ClearButton = (gcnew System::Windows::Forms::Button());
			this->LnButton = (gcnew System::Windows::Forms::Button());
			this->TextFieldPanel = (gcnew System::Windows::Forms::Panel());
			this->AngleLabel = (gcnew System::Windows::Forms::Label());
			this->OperationsListBox = (gcnew System::Windows::Forms::ListBox());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->AnsButton = (gcnew System::Windows::Forms::Button());
			this->InputTextBox = (gcnew System::Windows::Forms::TextBox());
			this->tableLayoutPanel7 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tableLayoutPanel2->SuspendLayout();
			this->tableLayoutPanel4->SuspendLayout();
			this->tableLayoutPanel3->SuspendLayout();
			this->tableLayoutPanel5->SuspendLayout();
			this->tableLayoutPanel6->SuspendLayout();
			this->ButtonsContainer->SuspendLayout();
			this->TextFieldPanel->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->tableLayoutPanel7->SuspendLayout();
			this->SuspendLayout();
			// 
			// AppNameLabel
			// 
			this->AppNameLabel->BackColor = System::Drawing::Color::Transparent;
			this->AppNameLabel->Cursor = System::Windows::Forms::Cursors::IBeam;
			this->AppNameLabel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->AppNameLabel->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->AppNameLabel->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 24, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic))));
			this->AppNameLabel->ForeColor = System::Drawing::Color::Aquamarine;
			this->AppNameLabel->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->AppNameLabel->Location = System::Drawing::Point(3, 0);
			this->AppNameLabel->Name = L"AppNameLabel";
			this->AppNameLabel->Size = System::Drawing::Size(478, 37);
			this->AppNameLabel->TabIndex = 0;
			this->AppNameLabel->Text = L"B I F R O S T ";
			this->AppNameLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// AppSubLabel
			// 
			this->AppSubLabel->BackColor = System::Drawing::Color::Transparent;
			this->AppSubLabel->Cursor = System::Windows::Forms::Cursors::IBeam;
			this->AppSubLabel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->AppSubLabel->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->AppSubLabel->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 22, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic))));
			this->AppSubLabel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->AppSubLabel->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->AppSubLabel->Location = System::Drawing::Point(3, 37);
			this->AppSubLabel->Name = L"AppSubLabel";
			this->AppSubLabel->Size = System::Drawing::Size(478, 38);
			this->AppSubLabel->TabIndex = 1;
			this->AppSubLabel->Text = L"C A L C U L A T O R";
			this->AppSubLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->AppSubLabel->UseMnemonic = false;
			// 
			// tableLayoutPanel2
			// 
			this->tableLayoutPanel2->BackColor = System::Drawing::Color::Black;
			this->tableLayoutPanel2->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::OutsetPartial;
			this->tableLayoutPanel2->ColumnCount = 1;
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				100)));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				20)));
			this->tableLayoutPanel2->Controls->Add(this->tableLayoutPanel4, 0, 1);
			this->tableLayoutPanel2->Controls->Add(this->tableLayoutPanel3, 0, 0);
			this->tableLayoutPanel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel2->Location = System::Drawing::Point(493, 3);
			this->tableLayoutPanel2->Name = L"tableLayoutPanel2";
			this->tableLayoutPanel2->RowCount = 2;
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 46.875F)));
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 53.125F)));
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel2->Size = System::Drawing::Size(138, 75);
			this->tableLayoutPanel2->TabIndex = 3;
			// 
			// tableLayoutPanel4
			// 
			this->tableLayoutPanel4->ColumnCount = 2;
			this->tableLayoutPanel4->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel4->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel4->Controls->Add(this->BaudLabel, 0, 0);
			this->tableLayoutPanel4->Controls->Add(this->BaudRateTextBox, 1, 0);
			this->tableLayoutPanel4->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel4->Location = System::Drawing::Point(6, 39);
			this->tableLayoutPanel4->Name = L"tableLayoutPanel4";
			this->tableLayoutPanel4->RowCount = 1;
			this->tableLayoutPanel4->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel4->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel4->Size = System::Drawing::Size(126, 30);
			this->tableLayoutPanel4->TabIndex = 1;
			// 
			// BaudLabel
			// 
			this->BaudLabel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->BaudLabel->Font = (gcnew System::Drawing::Font(L"Audiowide", 8, System::Drawing::FontStyle::Bold));
			this->BaudLabel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->BaudLabel->Location = System::Drawing::Point(3, 0);
			this->BaudLabel->Name = L"BaudLabel";
			this->BaudLabel->Size = System::Drawing::Size(57, 30);
			this->BaudLabel->TabIndex = 8;
			this->BaudLabel->Text = L"BAUD:";
			this->BaudLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// BaudRateTextBox
			// 
			this->BaudRateTextBox->BackColor = System::Drawing::SystemColors::MenuText;
			this->BaudRateTextBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->BaudRateTextBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->BaudRateTextBox->Font = (gcnew System::Drawing::Font(L"Audiowide", 11, System::Drawing::FontStyle::Bold));
			this->BaudRateTextBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->BaudRateTextBox->Location = System::Drawing::Point(66, 3);
			this->BaudRateTextBox->MaxLength = 200;
			this->BaudRateTextBox->Name = L"BaudRateTextBox";
			this->BaudRateTextBox->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->BaudRateTextBox->Size = System::Drawing::Size(57, 19);
			this->BaudRateTextBox->TabIndex = 6;
			this->BaudRateTextBox->Text = L"9600";
			this->BaudRateTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// tableLayoutPanel3
			// 
			this->tableLayoutPanel3->ColumnCount = 2;
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel3->Controls->Add(this->ComTextBox, 1, 0);
			this->tableLayoutPanel3->Controls->Add(this->PortLabel, 0, 0);
			this->tableLayoutPanel3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel3->Location = System::Drawing::Point(6, 6);
			this->tableLayoutPanel3->Name = L"tableLayoutPanel3";
			this->tableLayoutPanel3->RowCount = 1;
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel3->Size = System::Drawing::Size(126, 24);
			this->tableLayoutPanel3->TabIndex = 0;
			// 
			// ComTextBox
			// 
			this->ComTextBox->BackColor = System::Drawing::SystemColors::MenuText;
			this->ComTextBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->ComTextBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ComTextBox->Font = (gcnew System::Drawing::Font(L"Audiowide", 11, System::Drawing::FontStyle::Bold));
			this->ComTextBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->ComTextBox->Location = System::Drawing::Point(66, 3);
			this->ComTextBox->MaxLength = 200;
			this->ComTextBox->Name = L"ComTextBox";
			this->ComTextBox->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->ComTextBox->Size = System::Drawing::Size(57, 19);
			this->ComTextBox->TabIndex = 6;
			this->ComTextBox->Text = L"COM4";
			this->ComTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// PortLabel
			// 
			this->PortLabel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PortLabel->Font = (gcnew System::Drawing::Font(L"Audiowide", 8, System::Drawing::FontStyle::Bold));
			this->PortLabel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->PortLabel->Location = System::Drawing::Point(3, 0);
			this->PortLabel->Name = L"PortLabel";
			this->PortLabel->Size = System::Drawing::Size(57, 24);
			this->PortLabel->TabIndex = 7;
			this->PortLabel->Text = L"PORT:";
			this->PortLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// tableLayoutPanel5
			// 
			this->tableLayoutPanel5->BackColor = System::Drawing::Color::Transparent;
			this->tableLayoutPanel5->ColumnCount = 2;
			this->tableLayoutPanel5->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				77.40964F)));
			this->tableLayoutPanel5->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				22.59036F)));
			this->tableLayoutPanel5->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				144)));
			this->tableLayoutPanel5->Controls->Add(this->tableLayoutPanel6, 0, 0);
			this->tableLayoutPanel5->Controls->Add(this->tableLayoutPanel2, 2, 0);
			this->tableLayoutPanel5->Dock = System::Windows::Forms::DockStyle::Top;
			this->tableLayoutPanel5->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel5->Name = L"tableLayoutPanel5";
			this->tableLayoutPanel5->RowCount = 1;
			this->tableLayoutPanel5->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 44.92754F)));
			this->tableLayoutPanel5->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 55.07246F)));
			this->tableLayoutPanel5->Size = System::Drawing::Size(634, 81);
			this->tableLayoutPanel5->TabIndex = 4;
			// 
			// tableLayoutPanel6
			// 
			this->tableLayoutPanel6->ColumnCount = 1;
			this->tableLayoutPanel6->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel6->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel6->Controls->Add(this->AppSubLabel, 0, 0);
			this->tableLayoutPanel6->Controls->Add(this->AppNameLabel, 0, 0);
			this->tableLayoutPanel6->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel6->Location = System::Drawing::Point(3, 3);
			this->tableLayoutPanel6->Name = L"tableLayoutPanel6";
			this->tableLayoutPanel6->RowCount = 2;
			this->tableLayoutPanel6->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel6->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel6->Size = System::Drawing::Size(484, 75);
			this->tableLayoutPanel6->TabIndex = 5;
			// 
			// ButtonsContainer
			// 
			this->ButtonsContainer->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->ButtonsContainer->ColumnCount = 4;
			this->ButtonsContainer->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				25)));
			this->ButtonsContainer->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				25)));
			this->ButtonsContainer->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				25)));
			this->ButtonsContainer->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				25)));
			this->ButtonsContainer->Controls->Add(this->ParenthesisButton, 1, 2);
			this->ButtonsContainer->Controls->Add(this->ModulusButton, 3, 2);
			this->ButtonsContainer->Controls->Add(this->PowButton, 2, 2);
			this->ButtonsContainer->Controls->Add(this->SqrtButton, 0, 2);
			this->ButtonsContainer->Controls->Add(this->ZeroButton, 0, 6);
			this->ButtonsContainer->Controls->Add(this->OneButton, 0, 5);
			this->ButtonsContainer->Controls->Add(this->DotButton, 1, 6);
			this->ButtonsContainer->Controls->Add(this->TwoButton, 1, 5);
			this->ButtonsContainer->Controls->Add(this->SendButton, 3, 6);
			this->ButtonsContainer->Controls->Add(this->ThreeButton, 2, 5);
			this->ButtonsContainer->Controls->Add(this->SubstractButton, 3, 5);
			this->ButtonsContainer->Controls->Add(this->AddButton, 3, 4);
			this->ButtonsContainer->Controls->Add(this->MultiplyButton, 3, 3);
			this->ButtonsContainer->Controls->Add(this->SixButton, 2, 4);
			this->ButtonsContainer->Controls->Add(this->FiveButton, 1, 4);
			this->ButtonsContainer->Controls->Add(this->FourButton, 0, 4);
			this->ButtonsContainer->Controls->Add(this->SevenButton, 0, 3);
			this->ButtonsContainer->Controls->Add(this->EightButton, 1, 3);
			this->ButtonsContainer->Controls->Add(this->NineButton, 2, 3);
			this->ButtonsContainer->Controls->Add(this->SinButton, 0, 1);
			this->ButtonsContainer->Controls->Add(this->CosButton, 1, 1);
			this->ButtonsContainer->Controls->Add(this->TanButton, 2, 1);
			this->ButtonsContainer->Controls->Add(this->DivideButton, 3, 1);
			this->ButtonsContainer->Controls->Add(this->DelButton, 3, 0);
			this->ButtonsContainer->Controls->Add(this->PiButton, 2, 6);
			this->ButtonsContainer->Controls->Add(this->LogButton, 0, 0);
			this->ButtonsContainer->Controls->Add(this->ClearButton, 2, 0);
			this->ButtonsContainer->Controls->Add(this->LnButton, 1, 0);
			this->ButtonsContainer->Location = System::Drawing::Point(3, 337);
			this->ButtonsContainer->Name = L"ButtonsContainer";
			this->ButtonsContainer->RowCount = 7;
			this->ButtonsContainer->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 14.28513F)));
			this->ButtonsContainer->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 14.28514F)));
			this->ButtonsContainer->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 14.28921F)));
			this->ButtonsContainer->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 14.28513F)));
			this->ButtonsContainer->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 14.28513F)));
			this->ButtonsContainer->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 14.28513F)));
			this->ButtonsContainer->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 14.28513F)));
			this->ButtonsContainer->Size = System::Drawing::Size(628, 310);
			this->ButtonsContainer->TabIndex = 1;
			// 
			// ParenthesisButton
			// 
			this->ParenthesisButton->AutoSize = true;
			this->ParenthesisButton->BackColor = System::Drawing::Color::White;
			this->ParenthesisButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ParenthesisButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->ParenthesisButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ParenthesisButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->ParenthesisButton->FlatAppearance->BorderSize = 2;
			this->ParenthesisButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->ParenthesisButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 14));
			this->ParenthesisButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->ParenthesisButton->Location = System::Drawing::Point(160, 91);
			this->ParenthesisButton->Name = L"ParenthesisButton";
			this->ParenthesisButton->Size = System::Drawing::Size(151, 38);
			this->ParenthesisButton->TabIndex = 28;
			this->ParenthesisButton->Text = L"()";
			this->ParenthesisButton->UseVisualStyleBackColor = false;
			// 
			// ModulusButton
			// 
			this->ModulusButton->AutoSize = true;
			this->ModulusButton->BackColor = System::Drawing::Color::LightGreen;
			this->ModulusButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ModulusButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->ModulusButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ModulusButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->ModulusButton->FlatAppearance->BorderSize = 2;
			this->ModulusButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->ModulusButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ModulusButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->ModulusButton->Location = System::Drawing::Point(474, 91);
			this->ModulusButton->Name = L"ModulusButton";
			this->ModulusButton->Size = System::Drawing::Size(151, 38);
			this->ModulusButton->TabIndex = 27;
			this->ModulusButton->Text = L"%";
			this->ModulusButton->UseVisualStyleBackColor = false;
			// 
			// PowButton
			// 
			this->PowButton->AutoSize = true;
			this->PowButton->BackColor = System::Drawing::Color::SpringGreen;
			this->PowButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->PowButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->PowButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PowButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->PowButton->FlatAppearance->BorderSize = 2;
			this->PowButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->PowButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->PowButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->PowButton->Location = System::Drawing::Point(317, 91);
			this->PowButton->Name = L"PowButton";
			this->PowButton->Size = System::Drawing::Size(151, 38);
			this->PowButton->TabIndex = 26;
			this->PowButton->Text = L"x^y";
			this->PowButton->UseVisualStyleBackColor = false;
			// 
			// SqrtButton
			// 
			this->SqrtButton->AutoSize = true;
			this->SqrtButton->BackColor = System::Drawing::Color::SpringGreen;
			this->SqrtButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->SqrtButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->SqrtButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->SqrtButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->SqrtButton->FlatAppearance->BorderSize = 2;
			this->SqrtButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->SqrtButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SqrtButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->SqrtButton->Location = System::Drawing::Point(3, 91);
			this->SqrtButton->Name = L"SqrtButton";
			this->SqrtButton->Size = System::Drawing::Size(151, 38);
			this->SqrtButton->TabIndex = 24;
			this->SqrtButton->Text = L"√";
			this->SqrtButton->UseVisualStyleBackColor = false;
			// 
			// ZeroButton
			// 
			this->ZeroButton->AutoSize = true;
			this->ZeroButton->BackColor = System::Drawing::Color::Silver;
			this->ZeroButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ZeroButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->ZeroButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ZeroButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->ZeroButton->FlatAppearance->BorderSize = 2;
			this->ZeroButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->ZeroButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ZeroButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->ZeroButton->Location = System::Drawing::Point(3, 267);
			this->ZeroButton->Name = L"ZeroButton";
			this->ZeroButton->Size = System::Drawing::Size(151, 40);
			this->ZeroButton->TabIndex = 12;
			this->ZeroButton->Text = L"0";
			this->ZeroButton->UseVisualStyleBackColor = false;
			// 
			// OneButton
			// 
			this->OneButton->AutoSize = true;
			this->OneButton->BackColor = System::Drawing::Color::Silver;
			this->OneButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->OneButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->OneButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->OneButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->OneButton->FlatAppearance->BorderSize = 2;
			this->OneButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->OneButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->OneButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->OneButton->Location = System::Drawing::Point(3, 223);
			this->OneButton->Name = L"OneButton";
			this->OneButton->Size = System::Drawing::Size(151, 38);
			this->OneButton->TabIndex = 8;
			this->OneButton->Text = L"1";
			this->OneButton->UseVisualStyleBackColor = false;
			// 
			// DotButton
			// 
			this->DotButton->AutoSize = true;
			this->DotButton->BackColor = System::Drawing::Color::White;
			this->DotButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->DotButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->DotButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->DotButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->DotButton->FlatAppearance->BorderSize = 2;
			this->DotButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->DotButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->DotButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->DotButton->Location = System::Drawing::Point(160, 267);
			this->DotButton->Name = L"DotButton";
			this->DotButton->Size = System::Drawing::Size(151, 40);
			this->DotButton->TabIndex = 13;
			this->DotButton->Text = L".";
			this->DotButton->UseVisualStyleBackColor = false;
			// 
			// TwoButton
			// 
			this->TwoButton->AutoSize = true;
			this->TwoButton->BackColor = System::Drawing::Color::Silver;
			this->TwoButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->TwoButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->TwoButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->TwoButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->TwoButton->FlatAppearance->BorderSize = 2;
			this->TwoButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->TwoButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TwoButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->TwoButton->Location = System::Drawing::Point(160, 223);
			this->TwoButton->Name = L"TwoButton";
			this->TwoButton->Size = System::Drawing::Size(151, 38);
			this->TwoButton->TabIndex = 9;
			this->TwoButton->Text = L"2";
			this->TwoButton->UseVisualStyleBackColor = false;
			// 
			// SendButton
			// 
			this->SendButton->AutoSize = true;
			this->SendButton->BackColor = System::Drawing::Color::Aquamarine;
			this->SendButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->SendButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->SendButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->SendButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->SendButton->FlatAppearance->BorderSize = 2;
			this->SendButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->SendButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SendButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->SendButton->Location = System::Drawing::Point(474, 267);
			this->SendButton->Name = L"SendButton";
			this->SendButton->Size = System::Drawing::Size(151, 40);
			this->SendButton->TabIndex = 15;
			this->SendButton->Text = L"=";
			this->SendButton->UseVisualStyleBackColor = false;
			// 
			// ThreeButton
			// 
			this->ThreeButton->AutoSize = true;
			this->ThreeButton->BackColor = System::Drawing::Color::Silver;
			this->ThreeButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ThreeButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->ThreeButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ThreeButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->ThreeButton->FlatAppearance->BorderSize = 2;
			this->ThreeButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->ThreeButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ThreeButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->ThreeButton->Location = System::Drawing::Point(317, 223);
			this->ThreeButton->Name = L"ThreeButton";
			this->ThreeButton->Size = System::Drawing::Size(151, 38);
			this->ThreeButton->TabIndex = 10;
			this->ThreeButton->Text = L"3";
			this->ThreeButton->UseVisualStyleBackColor = false;
			// 
			// SubstractButton
			// 
			this->SubstractButton->AutoSize = true;
			this->SubstractButton->BackColor = System::Drawing::Color::LightGreen;
			this->SubstractButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->SubstractButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->SubstractButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->SubstractButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->SubstractButton->FlatAppearance->BorderSize = 2;
			this->SubstractButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->SubstractButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SubstractButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->SubstractButton->Location = System::Drawing::Point(474, 223);
			this->SubstractButton->Name = L"SubstractButton";
			this->SubstractButton->Size = System::Drawing::Size(151, 38);
			this->SubstractButton->TabIndex = 11;
			this->SubstractButton->Text = L"-";
			this->SubstractButton->UseVisualStyleBackColor = false;
			// 
			// AddButton
			// 
			this->AddButton->AutoSize = true;
			this->AddButton->BackColor = System::Drawing::Color::LightGreen;
			this->AddButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->AddButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->AddButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->AddButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->AddButton->FlatAppearance->BorderSize = 2;
			this->AddButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->AddButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->AddButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->AddButton->Location = System::Drawing::Point(474, 179);
			this->AddButton->Name = L"AddButton";
			this->AddButton->Size = System::Drawing::Size(151, 38);
			this->AddButton->TabIndex = 16;
			this->AddButton->Text = L"+";
			this->AddButton->UseVisualStyleBackColor = false;
			// 
			// MultiplyButton
			// 
			this->MultiplyButton->AutoSize = true;
			this->MultiplyButton->BackColor = System::Drawing::Color::LightGreen;
			this->MultiplyButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->MultiplyButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->MultiplyButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->MultiplyButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->MultiplyButton->FlatAppearance->BorderSize = 2;
			this->MultiplyButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->MultiplyButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->MultiplyButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->MultiplyButton->Location = System::Drawing::Point(474, 135);
			this->MultiplyButton->Name = L"MultiplyButton";
			this->MultiplyButton->Size = System::Drawing::Size(151, 38);
			this->MultiplyButton->TabIndex = 7;
			this->MultiplyButton->Text = L"*";
			this->MultiplyButton->UseVisualStyleBackColor = false;
			// 
			// SixButton
			// 
			this->SixButton->AutoSize = true;
			this->SixButton->BackColor = System::Drawing::Color::Silver;
			this->SixButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->SixButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->SixButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->SixButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->SixButton->FlatAppearance->BorderSize = 2;
			this->SixButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->SixButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SixButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->SixButton->Location = System::Drawing::Point(317, 179);
			this->SixButton->Name = L"SixButton";
			this->SixButton->Size = System::Drawing::Size(151, 38);
			this->SixButton->TabIndex = 6;
			this->SixButton->Text = L"6";
			this->SixButton->UseVisualStyleBackColor = false;
			// 
			// FiveButton
			// 
			this->FiveButton->AutoSize = true;
			this->FiveButton->BackColor = System::Drawing::Color::Silver;
			this->FiveButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->FiveButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->FiveButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->FiveButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->FiveButton->FlatAppearance->BorderSize = 2;
			this->FiveButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->FiveButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->FiveButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->FiveButton->Location = System::Drawing::Point(160, 179);
			this->FiveButton->Name = L"FiveButton";
			this->FiveButton->Size = System::Drawing::Size(151, 38);
			this->FiveButton->TabIndex = 5;
			this->FiveButton->Text = L"5";
			this->FiveButton->UseVisualStyleBackColor = false;
			// 
			// FourButton
			// 
			this->FourButton->AutoSize = true;
			this->FourButton->BackColor = System::Drawing::Color::Silver;
			this->FourButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->FourButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->FourButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->FourButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->FourButton->FlatAppearance->BorderSize = 2;
			this->FourButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->FourButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->FourButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->FourButton->Location = System::Drawing::Point(3, 179);
			this->FourButton->Name = L"FourButton";
			this->FourButton->Size = System::Drawing::Size(151, 38);
			this->FourButton->TabIndex = 4;
			this->FourButton->Text = L"4";
			this->FourButton->UseVisualStyleBackColor = false;
			// 
			// SevenButton
			// 
			this->SevenButton->AutoSize = true;
			this->SevenButton->BackColor = System::Drawing::Color::Silver;
			this->SevenButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->SevenButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->SevenButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->SevenButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->SevenButton->FlatAppearance->BorderSize = 2;
			this->SevenButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->SevenButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SevenButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->SevenButton->Location = System::Drawing::Point(3, 135);
			this->SevenButton->Name = L"SevenButton";
			this->SevenButton->Size = System::Drawing::Size(151, 38);
			this->SevenButton->TabIndex = 0;
			this->SevenButton->Text = L"7";
			this->SevenButton->UseVisualStyleBackColor = false;
			// 
			// EightButton
			// 
			this->EightButton->AutoSize = true;
			this->EightButton->BackColor = System::Drawing::Color::Silver;
			this->EightButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->EightButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->EightButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->EightButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->EightButton->FlatAppearance->BorderSize = 2;
			this->EightButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->EightButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->EightButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->EightButton->Location = System::Drawing::Point(160, 135);
			this->EightButton->Name = L"EightButton";
			this->EightButton->Size = System::Drawing::Size(151, 38);
			this->EightButton->TabIndex = 1;
			this->EightButton->Text = L"8";
			this->EightButton->UseVisualStyleBackColor = false;
			// 
			// NineButton
			// 
			this->NineButton->AutoSize = true;
			this->NineButton->BackColor = System::Drawing::Color::Silver;
			this->NineButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->NineButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->NineButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->NineButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->NineButton->FlatAppearance->BorderSize = 2;
			this->NineButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->NineButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->NineButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->NineButton->Location = System::Drawing::Point(317, 135);
			this->NineButton->Name = L"NineButton";
			this->NineButton->Size = System::Drawing::Size(151, 38);
			this->NineButton->TabIndex = 2;
			this->NineButton->Text = L"9";
			this->NineButton->UseVisualStyleBackColor = false;
			// 
			// SinButton
			// 
			this->SinButton->AutoSize = true;
			this->SinButton->BackColor = System::Drawing::Color::SpringGreen;
			this->SinButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->SinButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->SinButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->SinButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->SinButton->FlatAppearance->BorderSize = 2;
			this->SinButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->SinButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SinButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->SinButton->Location = System::Drawing::Point(3, 47);
			this->SinButton->Name = L"SinButton";
			this->SinButton->Size = System::Drawing::Size(151, 38);
			this->SinButton->TabIndex = 17;
			this->SinButton->Text = L"SIN";
			this->SinButton->UseVisualStyleBackColor = false;
			// 
			// CosButton
			// 
			this->CosButton->AutoSize = true;
			this->CosButton->BackColor = System::Drawing::Color::SpringGreen;
			this->CosButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->CosButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->CosButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->CosButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->CosButton->FlatAppearance->BorderSize = 2;
			this->CosButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->CosButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->CosButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->CosButton->Location = System::Drawing::Point(160, 47);
			this->CosButton->Name = L"CosButton";
			this->CosButton->Size = System::Drawing::Size(151, 38);
			this->CosButton->TabIndex = 18;
			this->CosButton->Text = L"COS";
			this->CosButton->UseVisualStyleBackColor = false;
			// 
			// TanButton
			// 
			this->TanButton->AutoSize = true;
			this->TanButton->BackColor = System::Drawing::Color::SpringGreen;
			this->TanButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->TanButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->TanButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->TanButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->TanButton->FlatAppearance->BorderSize = 2;
			this->TanButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->TanButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TanButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->TanButton->Location = System::Drawing::Point(317, 47);
			this->TanButton->Name = L"TanButton";
			this->TanButton->Size = System::Drawing::Size(151, 38);
			this->TanButton->TabIndex = 14;
			this->TanButton->Text = L"TAN";
			this->TanButton->UseVisualStyleBackColor = false;
			// 
			// DivideButton
			// 
			this->DivideButton->AutoSize = true;
			this->DivideButton->BackColor = System::Drawing::Color::LightGreen;
			this->DivideButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->DivideButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->DivideButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->DivideButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->DivideButton->FlatAppearance->BorderSize = 2;
			this->DivideButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->DivideButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->DivideButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->DivideButton->Location = System::Drawing::Point(474, 47);
			this->DivideButton->Name = L"DivideButton";
			this->DivideButton->Size = System::Drawing::Size(151, 38);
			this->DivideButton->TabIndex = 3;
			this->DivideButton->Text = L"/";
			this->DivideButton->UseVisualStyleBackColor = false;
			// 
			// DelButton
			// 
			this->DelButton->AutoSize = true;
			this->DelButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->DelButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->DelButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->DelButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->DelButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->DelButton->FlatAppearance->BorderSize = 2;
			this->DelButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->DelButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->DelButton->ForeColor = System::Drawing::Color::OrangeRed;
			this->DelButton->Location = System::Drawing::Point(474, 3);
			this->DelButton->Name = L"DelButton";
			this->DelButton->Size = System::Drawing::Size(151, 38);
			this->DelButton->TabIndex = 20;
			this->DelButton->Text = L"DEL";
			this->DelButton->UseVisualStyleBackColor = false;
			// 
			// PiButton
			// 
			this->PiButton->AutoSize = true;
			this->PiButton->BackColor = System::Drawing::Color::MediumSpringGreen;
			this->PiButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->PiButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->PiButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PiButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->PiButton->FlatAppearance->BorderSize = 2;
			this->PiButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->PiButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15));
			this->PiButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->PiButton->Location = System::Drawing::Point(317, 267);
			this->PiButton->Name = L"PiButton";
			this->PiButton->Size = System::Drawing::Size(151, 40);
			this->PiButton->TabIndex = 19;
			this->PiButton->Text = L"π";
			this->PiButton->UseVisualStyleBackColor = false;
			// 
			// LogButton
			// 
			this->LogButton->AutoSize = true;
			this->LogButton->BackColor = System::Drawing::Color::SpringGreen;
			this->LogButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->LogButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->LogButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->LogButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->LogButton->FlatAppearance->BorderSize = 2;
			this->LogButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->LogButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->LogButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->LogButton->Location = System::Drawing::Point(3, 3);
			this->LogButton->Name = L"LogButton";
			this->LogButton->Size = System::Drawing::Size(151, 38);
			this->LogButton->TabIndex = 23;
			this->LogButton->Text = L"LOG";
			this->LogButton->UseVisualStyleBackColor = false;
			// 
			// ClearButton
			// 
			this->ClearButton->AutoSize = true;
			this->ClearButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->ClearButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClearButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->ClearButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ClearButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->ClearButton->FlatAppearance->BorderSize = 2;
			this->ClearButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->ClearButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ClearButton->ForeColor = System::Drawing::Color::Red;
			this->ClearButton->Location = System::Drawing::Point(317, 3);
			this->ClearButton->Name = L"ClearButton";
			this->ClearButton->Size = System::Drawing::Size(151, 38);
			this->ClearButton->TabIndex = 22;
			this->ClearButton->Text = L"CLEAR";
			this->ClearButton->UseVisualStyleBackColor = false;
			// 
			// LnButton
			// 
			this->LnButton->AutoSize = true;
			this->LnButton->BackColor = System::Drawing::Color::SpringGreen;
			this->LnButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->LnButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->LnButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->LnButton->FlatAppearance->BorderColor = System::Drawing::Color::PaleTurquoise;
			this->LnButton->FlatAppearance->BorderSize = 2;
			this->LnButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->LnButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->LnButton->ForeColor = System::Drawing::SystemColors::MenuText;
			this->LnButton->Location = System::Drawing::Point(160, 3);
			this->LnButton->Name = L"LnButton";
			this->LnButton->Size = System::Drawing::Size(151, 38);
			this->LnButton->TabIndex = 25;
			this->LnButton->Text = L"LN";
			this->LnButton->UseVisualStyleBackColor = false;
			// 
			// TextFieldPanel
			// 
			this->TextFieldPanel->AutoSize = true;
			this->TextFieldPanel->BackColor = System::Drawing::Color::Black;
			this->TextFieldPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->TextFieldPanel->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->TextFieldPanel->Controls->Add(this->AngleLabel);
			this->TextFieldPanel->Controls->Add(this->OperationsListBox);
			this->TextFieldPanel->Controls->Add(this->tableLayoutPanel1);
			this->TextFieldPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->TextFieldPanel->Location = System::Drawing::Point(3, 3);
			this->TextFieldPanel->Name = L"TextFieldPanel";
			this->TextFieldPanel->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->TextFieldPanel->Size = System::Drawing::Size(628, 328);
			this->TextFieldPanel->TabIndex = 2;
			// 
			// AngleLabel
			// 
			this->AngleLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->AngleLabel->BackColor = System::Drawing::SystemColors::InfoText;
			this->AngleLabel->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->AngleLabel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->AngleLabel->Location = System::Drawing::Point(581, 0);
			this->AngleLabel->Name = L"AngleLabel";
			this->AngleLabel->Size = System::Drawing::Size(43, 24);
			this->AngleLabel->TabIndex = 8;
			this->AngleLabel->Text = L"⊾π";
			this->AngleLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// OperationsListBox
			// 
			this->OperationsListBox->BackColor = System::Drawing::SystemColors::InfoText;
			this->OperationsListBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->OperationsListBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->OperationsListBox->DrawMode = System::Windows::Forms::DrawMode::OwnerDrawVariable;
			this->OperationsListBox->Font = (gcnew System::Drawing::Font(L"Audiowide", 16, System::Drawing::FontStyle::Bold));
			this->OperationsListBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->OperationsListBox->FormattingEnabled = true;
			this->OperationsListBox->ItemHeight = 30;
			this->OperationsListBox->Location = System::Drawing::Point(0, 0);
			this->OperationsListBox->Name = L"OperationsListBox";
			this->OperationsListBox->Size = System::Drawing::Size(624, 284);
			this->OperationsListBox->TabIndex = 5;
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->BackColor = System::Drawing::Color::Black;
			this->tableLayoutPanel1->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Inset;
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				91.86228F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				8.137715F)));
			this->tableLayoutPanel1->Controls->Add(this->AnsButton, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->InputTextBox, 0, 0);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 284);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 1;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(624, 40);
			this->tableLayoutPanel1->TabIndex = 3;
			// 
			// AnsButton
			// 
			this->AnsButton->BackColor = System::Drawing::Color::SeaGreen;
			this->AnsButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->AnsButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->AnsButton->FlatAppearance->BorderColor = System::Drawing::Color::WhiteSmoke;
			this->AnsButton->FlatAppearance->BorderSize = 2;
			this->AnsButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->AnsButton->Font = (gcnew System::Drawing::Font(L"Bauhaus 93", 11));
			this->AnsButton->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->AnsButton->Location = System::Drawing::Point(574, 5);
			this->AnsButton->Name = L"AnsButton";
			this->AnsButton->Size = System::Drawing::Size(45, 30);
			this->AnsButton->TabIndex = 2;
			this->AnsButton->Text = L"ANS";
			this->AnsButton->UseVisualStyleBackColor = false;
			// 
			// InputTextBox
			// 
			this->InputTextBox->BackColor = System::Drawing::SystemColors::InfoText;
			this->InputTextBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->InputTextBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->InputTextBox->Font = (gcnew System::Drawing::Font(L"Audiowide", 18, System::Drawing::FontStyle::Bold));
			this->InputTextBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->InputTextBox->Location = System::Drawing::Point(5, 5);
			this->InputTextBox->MaxLength = 200;
			this->InputTextBox->Name = L"InputTextBox";
			this->InputTextBox->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->InputTextBox->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->InputTextBox->Size = System::Drawing::Size(561, 31);
			this->InputTextBox->TabIndex = 0;
			this->InputTextBox->Text = L"100 + 15";
			// 
			// tableLayoutPanel7
			// 
			this->tableLayoutPanel7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->tableLayoutPanel7->ColumnCount = 1;
			this->tableLayoutPanel7->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel7->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel7->Controls->Add(this->ButtonsContainer, 0, 1);
			this->tableLayoutPanel7->Controls->Add(this->TextFieldPanel, 0, 0);
			this->tableLayoutPanel7->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel7->Location = System::Drawing::Point(0, 81);
			this->tableLayoutPanel7->Name = L"tableLayoutPanel7";
			this->tableLayoutPanel7->RowCount = 2;
			this->tableLayoutPanel7->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 51.44404F)));
			this->tableLayoutPanel7->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 48.55596F)));
			this->tableLayoutPanel7->Size = System::Drawing::Size(634, 650);
			this->tableLayoutPanel7->TabIndex = 5;
			// 
			// CalculatorForm
			// 
			this->AccessibleDescription = L"Calculator";
			this->AccessibleName = L"Bifrost";
			this->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Silver;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(634, 731);
			this->Controls->Add(this->tableLayoutPanel7);
			this->Controls->Add(this->tableLayoutPanel5);
			this->DoubleBuffered = true;
			this->ForeColor = System::Drawing::SystemColors::ControlText;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximumSize = System::Drawing::Size(1920, 1080);
			this->MinimumSize = System::Drawing::Size(362, 558);
			this->Name = L"CalculatorForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Bifrost Calculator";
			this->Load += gcnew System::EventHandler(this, &CalculatorForm::CalculatorForm_Load);
			this->tableLayoutPanel2->ResumeLayout(false);
			this->tableLayoutPanel4->ResumeLayout(false);
			this->tableLayoutPanel4->PerformLayout();
			this->tableLayoutPanel3->ResumeLayout(false);
			this->tableLayoutPanel3->PerformLayout();
			this->tableLayoutPanel5->ResumeLayout(false);
			this->tableLayoutPanel6->ResumeLayout(false);
			this->ButtonsContainer->ResumeLayout(false);
			this->ButtonsContainer->PerformLayout();
			this->TextFieldPanel->ResumeLayout(false);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			this->tableLayoutPanel7->ResumeLayout(false);
			this->tableLayoutPanel7->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
