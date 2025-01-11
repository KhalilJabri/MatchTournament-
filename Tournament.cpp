#include "pch.h"
#include "Tournament.h"

#include <cliext/vector>
#include <cliext/map>
#include <cliext/utility>
#include <fstream>
#include <msclr/marshal_cppstd.h>


using namespace System;
using namespace System::Collections::Generic;
using namespace cliext;
using namespace System::IO;

// Constructor for Match struct Spielstruktur wird konstruiert
Tournament::Match::Match(String^ home, String^ away)
    : homeTeam(home), awayTeam(away), homeScore(-1), awayScore(-1) {
}

Tournament::Tournament() {
    teams = gcnew List<String^>();
    schedule = gcnew List<List<Match^>^>();
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
    if (teams == nullptr || teams->Count < 2) {
        return "Not enough teams to generate a schedule.";
    }

    Random^ random = gcnew Random(); // Random Objekt wurde erzeugt
    schedule = gcnew List<List<Match^>^>();

    // Shuffle teams list
    List<String^>^ shuffledTeams = gcnew List<String^>(teams);
    for (int i = 0; i < shuffledTeams->Count; i++) {
        int randomIndex = random->Next(i, shuffledTeams->Count);
        String^ temp = shuffledTeams[i];
        shuffledTeams[i] = shuffledTeams[randomIndex];
        shuffledTeams[randomIndex] = temp;
    }

    int numRounds = shuffledTeams->Count - 1;
    int numMatchesPerRound = shuffledTeams->Count / 2;

    // Generate schedule with random pairings
    for (int round = 0; round < numRounds; round++) {
        List<Match^>^ roundMatches = gcnew List<Match^>();

        // Copy the shuffled teams for pairing
        List<String^>^ tempTeams = gcnew List<String^>(shuffledTeams);

        while (tempTeams->Count >= 2) {
            int homeIndex = random->Next(tempTeams->Count);
            String^ homeTeam = tempTeams[homeIndex];
            tempTeams->RemoveAt(homeIndex);

            int awayIndex = random->Next(tempTeams->Count);
            String^ awayTeam = tempTeams[awayIndex];
            tempTeams->RemoveAt(awayIndex);

            roundMatches->Add(gcnew Match(homeTeam, awayTeam));
        }

        schedule->Add(roundMatches);
    }

    return "Randomized schedule generated successfully.";
}

// Function to save the schedule on files start
String^ Tournament::SaveSchedule() {
    for (int matchdayNumber = 0; matchdayNumber < schedule->Count; ++matchdayNumber) {
        String^ firstRound_filename = "matchday_" + (matchdayNumber + 1) + ".txt";
        String^ secondRound_filename = "matchday_" + (matchdayNumber + 18) + ".txt";
        StreamWriter^ firstRound_fileWriter = gcnew StreamWriter(firstRound_filename);
        StreamWriter^ secondRound_fileWriter = gcnew StreamWriter(secondRound_filename);

        for each (Match ^ match in schedule[matchdayNumber]) {
            firstRound_fileWriter->WriteLine(match->homeTeam + " vs " + match->awayTeam);
            secondRound_fileWriter->WriteLine(match->awayTeam + " vs " + match->homeTeam);
        }

        firstRound_fileWriter->Close();
        secondRound_fileWriter->Close();
    }
    return "Rangliste erfolgreich gespeichert";
}

// Function to display matches for a specific round
List<String^>^ Tournament::DisplayMatchday(int matchdayNumber) {
    List<String^>^ matchList = gcnew List<String^>();
    try {
        String^ filename = "matchday_" + (matchdayNumber)+".txt";
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
}

String^ Tournament::EnterMatchResults(List<String^>^ scores, String^ match_number) {
    String^ matchdayFile = "score_" + match_number + ".txt";
    StreamWriter^ scoreFile = gcnew StreamWriter(matchdayFile);

    for each (String ^ score_line in scores) {

        scoreFile->WriteLine(score_line);
    }

    scoreFile->Close();

    return "Results saved successfully in " + matchdayFile;
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

    for (int i = 0; i < matchs->Count - 1; i++) {
        for (int j = i + 1; j < matchs->Count; j++) {

            if (matchs[i]->score < matchs[j]->score) {
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

            // Update team B's score
            Tournament::MatchScore^ secondMatch = FindMatchScore(matchday, secondTeam);
            if (secondMatch == nullptr) {
                secondMatch = gcnew Tournament::MatchScore(secondTeam, 0);
                matchday->Add(secondMatch);
            }

			if (firstScore > secondScore) {
				firstMatch->AddNewGoals(3);
			}
			else if (firstScore < secondScore) {
				secondMatch->AddNewGoals(3);
			}
			else {
				firstMatch->AddNewGoals(1);
				secondMatch->AddNewGoals(1);
			}

        }

        scoreFile->Close();
    }

    matchday = SortMatch(matchday);

    return matchday;

    /*for each (Tournament::MatchScore ^ matchScore in matchday) {
        Console::WriteLine("{0}: {1} goals", matchScore->team, matchScore->score);
    }*/
}
