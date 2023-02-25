/******************************************************
** Program:     CS_162_Project1.cpp
** Author:      Benjamin Anderson II
** Date:        Jul 3, 2022
** Description: This is the first project given in CS_162, and is meant to test us on our understanding of 
                    pointers and ability to do basic algorithmic thinking
** Input:       file containing football data
** Output:      statistics on said football data
******************************************************/


#include "proj1_headers.h"
#include <map>

void first_year(School*, int);        // Finds the founding year for the school's team
void best_year(School*, int);         // Finds the school's best year in terms of total points scored 
void sort_schools(School*, int);      // Sorts the schools by their win ratios (best to worst)

void calculateWinRatio(School*, int); // Calculates the win ratio for the school over recorded history
string improved_substr(string &);     //helper function used to store file data into variables

int main(int argc, char const *argv[]){
    
    School* schools;
    int numOfSchools;
    ifstream myFile;


    myFile.open("SampleData.txt");
    string myLine;
    if(myFile.is_open()){

        getline(myFile, myLine); //Reads the num of Schools

        numOfSchools = stoi(myLine);
        schools = create_school_array(numOfSchools);
        for(int i = 0; i < numOfSchools; i++){
            load_school_data(schools, i, myFile);
        }
    } else { // When the filel can't be opened
        cout << "Could Not Open File\n";
    }
    myFile.close();

    //get user input on which option to choose
    while(true){
        string option;

        cout << "Which option would you like to choose?" << endl;
        cout << "Display each school's earliest year on file (Press 1):" << endl;
        cout << "List the best overall year (by points aquired) for each school (Press 2):" << endl;
        cout << "Sort schools by overall historical performance (win ratio) (Press 3):" << endl;
        cout << "Quit (Press 4):" << endl;
        cin >> option;
        try{
            int op = stoi(option);
            switch(op){
            case 1:
                first_year(schools, numOfSchools);
                break;
            case 2:
                best_year(schools, numOfSchools);
                break;
            case 3:
                sort_schools(schools, numOfSchools);
                break;
            case 4:
                cout << "Goodbye!" << endl;
                goto stop;
            default:
                cout << "Invalid Number!" << endl;
                break;
            }
        } catch(...){
            cout << "Invalid Input, please type a number!\n\n";
            continue;
        }
        
    }
    stop:;
    delete_school_data(schools);
    return 0;
}

School* create_school_array(int numOfSchools){
    return new School[numOfSchools];
}

Match* create_match_array(int numOfMatches){
    return new Match[numOfMatches];
}

void load_school_data(School* schools, int index, ifstream &file){
    string line;
    getline(file, line);

    (schools+index)->name = improved_substr(line);
    (schools+index)->moniker = improved_substr(line);
    (schools+index)->city = improved_substr(line);
    (schools+index)->numMatches = stoi(improved_substr(line));
    (schools+index)->array = create_match_array((schools+index)->numMatches);
    for(int i = 0; i < (schools+index)->numMatches; i++){
        load_match_data((schools+index)->array, i, file);
    }
    calculateWinRatio(schools, index);
}

void load_match_data(Match* matches, int index, ifstream &file){
    string line;
    getline(file, line);

    (matches+index)->year = (unsigned int)stoi(improved_substr(line));
    (matches+index)->homeScore = (unsigned int)stoi(improved_substr(line));
    (matches+index)->opponentName = improved_substr(line);
    (matches+index)->opponentScore = (unsigned int)stoi(improved_substr(line));
}

void delete_school_data(School* schools){
    delete[] schools;
}

//Idea from  https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
string improved_substr(string &str){
    string delimiter = ";";
    string result = str.substr(0, str.find(delimiter));     // gets the substring from start to the delimeter
    str.erase(0, str.find(delimiter) + delimiter.length()); // erases the first part of the string including the delimiting character
    return result;
}

void first_year(School *schoolList, int size){
    for(int i = 0; i < size; i++){//loops through all of the schools
        string toPrint = "";//-----------------------------------String to be printed
        toPrint += (schoolList+i)->name + ", \"" +
            (schoolList+i)->moniker + "\", Team Founded: ";
        unsigned int earliest_year = 999999; //------------------start at a high number so that the first item in the array will always be lower
        for (int j = 0; j < (schoolList+i)->numMatches; j++){ // loops through all of the matches within the school
            Match* arr = (schoolList+i) -> array;
            if( (arr+j) -> year < earliest_year){ //-------------if the current year is before the "earliest year replace the earliest year"
                earliest_year = (arr+j) -> year;
            }
        }
        toPrint += to_string((int)earliest_year) + ", City: " + (schoolList+i) -> city;
        cout << toPrint << endl;
    }
    cout << endl;
}

void best_year(School* schoolList, int size){
    for(int i = 0; i < size; i++){ //--------------------------------------loops through all of the schools
        Match* currMatches = (schoolList+i)->array;
        string toPrint = ""; //-------------------String to be printed
        toPrint += (schoolList+i)->name + ": ";
        map<unsigned int, unsigned int> year_score_map; //-----------------{year, homeScore}
        for(int j = 0; j < (schoolList+i)->numMatches; j++){ //------------loops through all of the matches in the school
            unsigned int matchYear = (currMatches+j)->year;
            unsigned int matchScore = (currMatches+j)->homeScore;
            if(year_score_map.find(matchYear) == year_score_map.end()){ //-if the year is not already in the map
                year_score_map[matchYear] = matchScore; //-----------------creates the key value pair for the given year
            } else {
                year_score_map[matchYear] += matchScore; //----------------increases the value by the matchScore
            }
        }

        unsigned int best_year = 0; 
        unsigned int best_score = 0; // starts at zero so that the first item in the map will be higher or equal

        map<unsigned int, unsigned int>::iterator it; 

        for(it = year_score_map.begin(); it != year_score_map.end(); it++){ //loop through the map to find the highest scoring year
            if(best_score < it->second){
                best_year = it->first;
                best_score = it->second;
            }
        }
        toPrint += to_string((int)best_score) + " points in " + to_string((int)best_year);
        cout << toPrint <<endl;
    }
    cout << endl;
}

//understanding of sorting in maps from    https://linuxhint.com/map-sort-key-c/
void sort_schools(School* schoolList, int size){
    map<double, string, greater<double>> ratio_name_map; //-------------------{winRatio, name, (sort so that the first item is greatest)}
    for(int i = 0; i < size; i++){//------------------------------------------Load the map data
        ratio_name_map[(schoolList+i)->winRatio] = (schoolList+i)->name;
    }
    map<double, string>::iterator it;
    for(it = ratio_name_map.begin(); it != ratio_name_map.end(); it++){ // loop through the map and print value, then key
        cout << it->second << ": " << it->first << endl;
    }
}

void calculateWinRatio(School* schools, int index){ 
    double total_wins = 0;
    double total_losses = 0;
    double total_ties = 0;
    Match* schoolMatches = (schools+index)->array;
    for(int i = 0; i < (schools+index)->numMatches; i++){
        unsigned int home = (schoolMatches+i)->homeScore;
        unsigned int opp = (schoolMatches+i)->opponentScore;
        if(home>opp) total_wins++;
        if(home<opp) total_losses++;
        if(home==opp) total_ties++;
    }
    (schools+index)->winRatio = total_wins / (total_wins + total_losses + total_ties); //application of the given formula
}