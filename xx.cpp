#include "pch.h"
#include "MyForm.h"


#include <cliext/vector>
#include <cliext/map>
#include <cliext/utility>
#include <fstream>
#include <string>
#include <msclr/marshal_cppstd.h>
#include <iostream>

using namespace System;
using namespace System::Collections::Generic;
using namespace cliext;
using namespace System::IO;
using namespace System::Windows::Forms;


ref class cvTournament {
private:
    ref struct Match {
        String^ homeTeam;
        String^ awayTeam;
        int homeScore;
        int awayScore;

        Match(String^ home, String^ away) {
            homeTeam = home;
            awayTeam = away;
            homeScore = -1;
            awayScore = -1;
        }
    };

    ref struct MatchScore {
        String^ team;
        int score;

        MatchScore(String^ teamNote, int scoreNote) {
            team = teamNote;
            score = scoreNote;
        }
    };

    List<String^>^ teams;
    List<List<Match^>^>^ schedule;

public:
    cvTournament() {
        teams = gcnew List<String^>();
        schedule = gcnew List<List<Match^>^>();
    }

    void CreateTeamsFile(String^ filename) {
        StreamWriter^ writer = gcnew StreamWriter(filename);
        Console::WriteLine("Enter the number of teams:");
        int numTeams;
        Int32::TryParse(Console::ReadLine(), numTeams);

        for (int i = 0; i < numTeams; i++) {
            Console::Write("Enter team name " + (i + 1) + ": ");
            String^ teamName = Console::ReadLine();
            writer->WriteLine(teamName);
            teams->Add(teamName);
        }

        writer->Close();
        Console::WriteLine("Teams saved to file.");
    }

    void ReadTeams(String^ filename) {
        StreamReader^ reader = gcnew StreamReader(filename);
        String^ line;

        while ((line = reader->ReadLine()) != nullptr) {
            teams->Add(line);
            Console::WriteLine(line);
        }
        reader->Close();
    }

    void GenerateSchedule() {
        if (teams == nullptr || teams->Count < 2) {
            Console::WriteLine("Not enough teams to generate a schedule.");
            return;
        }

        int numTeams = teams->Count;

        if (numTeams % 2 != 0) {
            Console::WriteLine("Number of teams must be even. Adding a bye week");
            teams->Add("successor");
            numTeams++;
        }

        for (int round = 0; round < numTeams - 1; ++round) {
            List<Match^>^ matchday = gcnew List<Match^>();

            for (int i = 0; i < numTeams / 2; ++i) {
                String^ home;
                String^ away;

                if (i == 0) {
                    // Anchor team remains fixed.
                    home = teams[0];
                    away = teams[(round + 1) % (numTeams - 1) + 1];
                }
                else {
                    // Rotate other teams.
                    home = teams[(i + round) % (numTeams - 1) + 1];
                    away = teams[(numTeams - i + round - 1) % (numTeams - 1) + 1];
                }

                // Alternate home/away to ensure fairness.
                if (round % 2 == 1 && i == 0) {
                    String^ temp = home;
                    home = away;
                    away = temp;
                }

                matchday->Add(gcnew Match(home, away));
            }
            /*Console::WriteLine("Round " + round + ":");
            Console::WriteLine("Matchday contains:");
            for each(Match ^ match in matchday) {
                Console::WriteLine("  " + match->homeTeam + " vs " + match->awayTeam);
            }*/
            schedule->Add(matchday);
        }

        Console::WriteLine("Schedule generated successfully");
    }

    void SaveSchedule() {
        for (int round = 0; round < schedule->Count; ++round) {

            String^ filename = "matchday_" + (round + 1) + ".txt";

            StreamWriter^ writer = gcnew StreamWriter(filename);

            for each (Match ^ match in schedule[round]) {
                writer->WriteLine(match->homeTeam + " vs " + match->awayTeam);
            }

            writer->Close();
        }
        Console::WriteLine("schedule saved successfully");
    }



    void DisplayMatchday(int round) {
        if (round < 1 || round > schedule->Count) {
            Console::WriteLine("Invalid round number.");
            return;
        }

        Console::WriteLine("Round " + round + ":");
        for each (Match ^ match in schedule[round - 1]) {
            Console::Write(match->homeTeam + " vs " + match->awayTeam);
            if (match->homeScore != -1 && match->awayScore != -1) {
                Console::Write(" (" + match->homeScore + " - " + match->awayScore + ")");
            }
            Console::WriteLine();
        }
    }

    void EnterMatchResults(String^ matchdayFile) {

        String^ scoreFileBase = matchdayFile->Replace("matchday_", "score_");
        scoreFileBase = scoreFileBase->Replace(".txt", "");

        int fileNumber = 1;
        String^ scoreFile = scoreFileBase + ".txt";

        StreamReader^ inputFile;
        StreamWriter^ outputFile;
        try {
            inputFile = gcnew StreamReader(matchdayFile);
            outputFile = gcnew StreamWriter(scoreFile);
        }
        catch (Exception^ ex) {
            Console::WriteLine("file does not exit {0}", ex->Message);
            return;
        }

        Console::WriteLine("Enter results for matches in {0} (format: TeamA-TeamB scoreA scoreB):", matchdayFile);

        String^ line;
        while ((line = inputFile->ReadLine()) != nullptr) {
            if (String::IsNullOrWhiteSpace(line)) continue;

            // Parse the match line (assumes "TeamA vs TeamB" format)
            array<String^>^ parts = line->Split(gcnew array<String^> { " vs " }, StringSplitOptions::None);
            if (parts->Length != 2) {
                Console::WriteLine("Error: Invalid match format in file: {0}", line);
                continue;
            }

            String^ teamA = parts[0]->Trim();
            String^ teamB = parts[1]->Trim();

            int scoreA, scoreB;
            Console::Write("{0} vs {1}: ", teamA, teamB);
            String^ input = Console::ReadLine();
            array<String^>^ scores = input->Split(gcnew array<wchar_t> { ' ' }, StringSplitOptions::RemoveEmptyEntries);

            if (scores->Length != 2 ||
                !Int32::TryParse(scores[0], scoreA) ||
                !Int32::TryParse(scores[1], scoreB)) {
                Console::WriteLine("Error: Invalid score input. Format should be: scoreA scoreB");
                continue;
            }

            outputFile->WriteLine("{0},{1},{2},{3}", teamA, teamB, scoreA, scoreB);
        }

        inputFile->Close();
        outputFile->Close();

        Console::WriteLine("Results saved successfully in {0}.", scoreFile);
    }

    void TriSelection() {

    }

};

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
                tournament->EnterMatchResults(matchdayFile);
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
