#pragma once

#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Collections::Generic;

ref class Tournament {
//public:
public:
    ref struct MatchScore {
        String^ team;
        int score;

        MatchScore(String^ teamNote, int scoreNote) {
            team = teamNote;
            score = scoreNote;
        }

        void AddNewGoals(int goals) {
            score += goals;
        }
    };


private:
    // Nested Match struct
    ref struct Match {
        String^ homeTeam;
        String^ awayTeam;
        int homeScore;
        int awayScore;

        Match(String^ home, String^ away);
    };

    List<String^>^ teams;
    List<List<Match^>^>^ schedule;


public:
    property List<String^>^ Teams {
        List<String^>^ get() { return teams; }
    }


public:
    Tournament();

    void CreateTeamsFile(String^ filename);
    void ReadTeams(String^ filename);
    String^ GenerateSchedule();
    String^ SaveSchedule();
    List<String^>^ DisplayMatchday(int round);
    //String^ EnterMatchResults(String^ matchdayFile);
    String^ EnterMatchResults(List<String^>^ scores, String^ match_number);
    List<Tournament::MatchScore^>^ DisplayRankings(int fileNumero);
};
