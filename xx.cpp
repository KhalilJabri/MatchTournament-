#include "pch.h"
#include "MyForm.h"


#include <cliext/vector>
#include <cliext/map>
#include <cliext/utility>
#include <fstream>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace cliext;
using namespace System::IO;
using namespace System::Windows::Forms;


int main() {
    Tournament^ tournament = gcnew Tournament();
    String^ teamFile = "teams.txt";
    //String^ scheduleFile = "matchday.txt";

    Application::SetCompatibleTextRenderingDefault(false);
    Application::EnableVisualStyles();
    xx::MyForm frm;
    Application::Run(% frm);


    int choice;
    do {
        Console::WriteLine("\nMenu:");
        Console::WriteLine("1. Create Teams File");
        Console::WriteLine("2. Read Teams");
        Console::WriteLine("3. Generate Schedule");
        Console::WriteLine("4. Save Schedule");
        Console::WriteLine("5. Display Matchday");
        Console::WriteLine("6. Enter Results");
        Console::WriteLine("7. Display Standings");
        Console::WriteLine("8. Exit");
        Console::Write("Enter your choice: ");
        Int32::TryParse(Console::ReadLine(), choice);

        switch (choice) {
        /*case 1:
            tournament->CreateTeamsFile(teamFile);
            break;*/
        case 2:
            tournament->ReadTeams(teamFile);
            break;
        case 3:
            tournament->GenerateSchedule();
            break;
        case 4:
            tournament->SaveSchedule();
            break;
        case 5: {
            int round;
            Console::Write("Enter round number: ");
            Int32::TryParse(Console::ReadLine(), round);
            tournament->DisplayMatchday(round);
            break;
        }
        case 6: {
            int matchday;
            Console::Write("Enter matchday number: ");

            if (Int32::TryParse(Console::ReadLine(), matchday)) {
                String^ matchdayFile = String::Format("matchday_{0}.txt", matchday);
                //tournament->EnterMatchResults(matchdayFile);
            }
            else {
                Console::WriteLine("Error: Invalid matchday number. Please enter a valid number.");
            }
            break;
        }
        case 7: {
            int fileNumero;
            Console::Write("Enter round number: ");
            Int32::TryParse(Console::ReadLine(), fileNumero);
            tournament->DisplayRankings(fileNumero);
            break;
        }
        case 8:
            Console::WriteLine("Exiting...");
            break;
        default:
            Console::WriteLine("Invalid choice, Try again!!!!");
        }
    } while (choice != 8);
    
    return 0;
}
