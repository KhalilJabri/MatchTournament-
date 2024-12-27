#pragma once
#include "Tournament.h"


namespace xx {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MyForm : public System::Windows::Forms::Form
	{

	private: Tournament^ tournament;


	public:
		MyForm(void)
		{
			InitializeComponent();
			tournament = gcnew Tournament();
		}

	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::Button^ button5;
	private: System::Windows::Forms::Button^ button6;




	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(152, 37);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(121, 40);
			this->button1->TabIndex = 0;
			this->button1->Text = L"read teams";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click_1);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(107, 203);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox1->Size = System::Drawing::Size(470, 222);
			this->textBox1->TabIndex = 1;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(293, 37);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(108, 40);
			this->button2->TabIndex = 2;
			this->button2->Text = L"Generate schedule";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(152, 102);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(121, 40);
			this->button3->TabIndex = 3;
			this->button3->Text = L"Display matchday";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(432, 37);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(111, 40);
			this->button4->TabIndex = 4;
			this->button4->Text = L"Save schedule";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(293, 102);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(108, 40);
			this->button5->TabIndex = 5;
			this->button5->Text = L"Enter Match results";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(432, 102);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(111, 40);
			this->button6->TabIndex = 6;
			this->button6->Text = L"Display ranking";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &MyForm::button6_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(749, 448);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {

	}

	private: System::Void button1_Click_1(System::Object^ sender, System::EventArgs^ e) {
		textBox1->ReadOnly = false;
		String^ filename = textBox1->Text;
		textBox1->Clear();

		try {
			tournament->ReadTeams(filename);

			for each (String ^ team in tournament->Teams) {
				textBox1->AppendText(team + Environment::NewLine);
			}
			textBox1->ReadOnly = true;
		}
		catch (Exception^ ex) {
			textBox1->AppendText("Error: you should write the file name");
		}

	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		textBox1->Clear();
		textBox1->ReadOnly = true;
		String^ msg = tournament->GenerateSchedule();
		textBox1->AppendText(msg);
	}

	private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {
		textBox1->Clear();
		textBox1->ReadOnly = true;
		String^ msg = tournament->SaveSchedule();
		textBox1->AppendText(msg);
	}

	private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
		String^ numberMatchday = textBox1->Text;

		try {
			List<String^>^ matchList = tournament->DisplayMatchday(Int32::Parse(numberMatchday));
			textBox1->Clear();
			for each (String ^ match in matchList) {
				textBox1->AppendText(match + Environment::NewLine);
			}
			textBox1->ReadOnly = true;
		}
		catch (Exception^ ex) {
			textBox1->ReadOnly = false;
			textBox1->Clear();
			textBox1->AppendText("Error: you should enter the number of matchday you want to see");
		}

	}

	private: System::Void button5_Click(System::Object^ sender, System::EventArgs^ e) {
	}

	private: System::Void button6_Click(System::Object^ sender, System::EventArgs^ e) {
		String^ numberMatchday = textBox1->Text;

		try {
			List<Tournament::MatchScore^>^ matchList = tournament->DisplayRankings(Int32::Parse(numberMatchday));
			textBox1->Clear();
			for each (Tournament::MatchScore^ matchScore in matchList) {
				String^ matchResult = matchScore->team + " : " + matchScore->score + "goals";
				textBox1->AppendText(matchResult + Environment::NewLine);
			}

			textBox1->ReadOnly = true;
		}
		catch (Exception^ ex) {
			textBox1->ReadOnly = false;
			textBox1->Clear();
			textBox1->AppendText("Error: you need to enter the match day number where you want to see the score!!");
		}
	}
};
}
