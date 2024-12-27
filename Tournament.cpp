#include "pch.h"
#include "Tournament.h"

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

// Constructor for Match struct
Tournament::Match::Match(String^ home, String^ away)
    : homeTeam(home), awayTeam(away), homeScore(-1), awayScore(-1) {
}

//Tournament::MatchScore::MatchScore(String^ teamNote, int scoreNote) {
//    team = teamNote;
//    score = scoreNote;
//}
//
//void Tournament::MatchScore::AddNewGoals(int goals) {
//    score += goals;
//}

Tournament::Tournament() {
    teams = gcnew List<String^>();
    schedule = gcnew List<List<Match^>^>();
}

// Function to create a file with teams name
void Tournament::CreateTeamsFile(String^ filename) {
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

// Function to read team from file text
void Tournament::ReadTeams(String^ filename) {
    StreamReader^ file = gcnew StreamReader(filename);
    String^ line;

    while ((line = file->ReadLine()) != nullptr) {
        teams->Add(line);
        Console::WriteLine(line);
    }
    file->Close();
}

// Function for Generating Schedule
String^ Tournament::GenerateSchedule() {

    int teamsNumber = teams->Count;

    if (teamsNumber % 2 != 0) {
        Console::WriteLine("the number of teams should be divided by 2, that's why we'll add another imagination word ( passed ) to tell that team is passed");
        teams->Add("Passed");
        teamsNumber++;
    }

    for (int teamRound = 0; teamRound < teamsNumber - 1; ++teamRound) {
        List<Match^>^ matchday = gcnew List<Match^>();

        for (int i = 0; i < teamsNumber / 2; ++i) {
            String^ home;
            String^ away;
            
            if (i == 0) {
                home = teams[0];
                away = teams[(teamRound + 1) % (teamsNumber - 1) + 1];
            }
            else {
                home = teams[(i + teamRound) % (teamsNumber - 1) + 1];
                away = teams[(teamsNumber - i + teamRound - 1) % (teamsNumber - 1) + 1];
            }

            if (teamRound % 2 == 1 && i == 0) {
                String^ temp = home;
                home = away;
                away = temp;
            }

            matchday->Add(gcnew Match(home, away));
        }
        schedule->Add(matchday);
    }

    return "Schedule generated successfully";
}

// Function to save the schedule on files start
String^ Tournament::SaveSchedule() {
    for (int matchdayNumber = 0; matchdayNumber < schedule->Count; ++matchdayNumber) {
        String^ filename = "matchday_" + (matchdayNumber + 1) + ".txt";
        StreamWriter^ fileWriter = gcnew StreamWriter(filename);

        for each (Match ^ match in schedule[matchdayNumber]) {
            fileWriter->WriteLine(match->homeTeam + " vs " + match->awayTeam);
        }

        fileWriter->Close();
    }
    return "Schedule saved successfully";
}

// Function to display matches for a specific round
List<String^>^ Tournament::DisplayMatchday(int matchdayNumber) {
    List<String^>^ matchList = gcnew List<String^>();
    try {
        String^ filename = "matchday_" + (matchdayNumber) + ".txt";
        StreamReader^ file = gcnew StreamReader(filename);
        String^ line;

        while ((line = file->ReadLine()) != nullptr) {
            matchList->Add(line);
            Console::WriteLine(line);
        }
        file->Close();
        return matchList;
    }
    catch (Exception^ e) {
        return nullptr;
    }

    /*if (round < 1 || round > schedule->Count) {
        Console::WriteLine("Invalid round number.");
        return nullptr;
    }*/

    //for each (Match ^ match in schedule[round - 1]) {
    //    //Console::Write(match->homeTeam + " vs " + match->awayTeam);
    //    matchList->Add(match->homeTeam + " vs " + match->awayTeam);
    //    //if (match->homeScore != -1 && match->awayScore != -1) {
    //    //    Console::Write(" (" + match->homeScore + " - " + match->awayScore + ")");
    //    //}
    //    //Console::WriteLine();
    //}
}

void Tournament::EnterMatchResults(String^ matchdayFile) {

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
        Console::WriteLine("Error: Unable to open file(s). Exception: {0}", ex->Message);
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

Tournament::MatchScore^ FindMatchScore(List<Tournament::MatchScore^>^ matchs, String^ teamName) {
    for each (Tournament::MatchScore ^ match in matchs) {
        if (match->team->Equals(teamName)) {
            return match;
        }
    }
    return nullptr;
}

List<Tournament::MatchScore^>^ SortMatch(List<Tournament::MatchScore^>^ matchs) {
    Tournament::MatchScore^ aux;

    for(int i = 0; i < matchs->Count-1; i++) {
        for (int j = i+1; j < matchs->Count; j++) {

            if ( matchs[i]->score < matchs[j]->score ) {
                aux = matchs[i];
                matchs[i] = matchs[j];
                matchs[j] = aux;
            }
        }
    }
    return matchs;
}


List<Tournament::MatchScore^>^ Tournament::DisplayRankings(int fileNumero) {
    List<Tournament::MatchScore^>^ matchday = gcnew List<Tournament::MatchScore^>();

    // Process each file from score_1 to score_fileNumber
    for (int i = 1; i <= fileNumero; i++) {
        String^ fileName = "score_" + i + ".txt";

        if (!File::Exists(fileName)) {
            continue;
        }

        StreamReader^ scoreFile = gcnew StreamReader(fileName);
        String^ line;

        while ((line = scoreFile->ReadLine()) != nullptr) {
            array<String^>^ partitions = line->Split(',');

            String^ firstTeam = partitions[0]->Trim();
            String^ secondTeam = partitions[1]->Trim();
            int firstScore = Int32::Parse(partitions[2]);
            int secondScore = Int32::Parse(partitions[3]);

            // Update team A's score
            Tournament::MatchScore^ firstMatch = FindMatchScore(matchday, firstTeam);
            if (firstMatch == nullptr) {
                firstMatch = gcnew Tournament::MatchScore(firstTeam, 0);
                matchday->Add(firstMatch);
            }
            firstMatch->AddNewGoals(firstScore);

            // Update team B's score
            Tournament::MatchScore^ secondMatch = FindMatchScore(matchday, secondTeam);
            if(secondMatch == nullptr) {
                secondMatch = gcnew Tournament::MatchScore(secondTeam, 0);
                matchday->Add(secondMatch);
            }
            secondMatch->AddNewGoals(secondScore);
        }

        scoreFile->Close();
    }

    matchday = SortMatch(matchday);

    return matchday;

    /*for each (Tournament::MatchScore ^ matchScore in matchday) {
        Console::WriteLine("{0}: {1} goals", matchScore->team, matchScore->score);
    }*/
}


