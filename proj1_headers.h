#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Match {
    unsigned int year;
    unsigned int homeScore;
    string opponentName;
    unsigned int opponentScore;
};

struct School {
    string name;
    string moniker;
    string city;
    double winRatio;
    int numMatches;
    struct Match* array;
};

School* create_school_array(int);
void load_school_data(School*, int, ifstream &);
Match* create_match_array(int);
void load_match_data(Match*, int, ifstream &); 
void delete_school_data(School*);