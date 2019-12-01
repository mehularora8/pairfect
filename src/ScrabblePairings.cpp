//TODO: Entering names and scores this round
//TODO: who wins? -> vector of pair/map of names to keep track of games
//TODO: ordering the vector of every round

#include <iostream>
#include <fstream>
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "strlib.h"
#include "lexicon.h"

using namespace std;

//Player struct
struct player{

    string name;
    string code;
    int    wins;
    int    spread;
    player(string name);
    friend bool operator <(const player&p1, const player&p2);
    friend bool operator ==(const player& p1, const player& p2);
    friend bool operator <=(const player& p1, const player& p2);
    friend bool operator >(const player& p1, const player& p2);
    friend bool operator >=(const player& p1, const player& p2);
    friend bool operator !=(const player& p1, const player& p2);
};

//Player struct member function and operator definitions
player::player(string name){
    this->name = name;
    code = "";
    wins = 0;
    spread = 0;
}

bool operator <(const player& p1, const player& p2) {
    return p1.wins < p2.wins|| (p1.wins == p2.wins && p1.spread < p2.spread);
}

bool operator ==(const player& p1, const player& p2) {
    return p1.wins == p2.wins && p1.spread == p2.spread;
}

bool operator <=(const player& p1, const player& p2) {
    return (p1 < p2) || (p1 == p2);
}

bool operator >(const player& p1, const player& p2) {
    return !(p1 <= p2);
}


bool operator >=(const player& p1, const player& p2) {
    return (p1 > p2) || (p1 == p2);
}


bool operator !=(const player& p1, const player& p2) {
    return !(p1 == p2);
}

//Round struct
struct roundNode{
    Vector<player> playerOrder;
    string    textFile; //Name of text file
    roundNode* next;
    roundNode* prev;

    roundNode(){
        next = nullptr;
        prev = nullptr;
    }
};

//Tournament class
class tourney{
private:
    roundNode* _first;
    roundNode* _last;
    int _noParticipants;
    Lexicon _participants;
    bool _newTourney;
    string _name;
    void australianDraw(Vector<player>& players);
    int _roundNo;
    void startNew();
    void menu();
    void incrementRound();
    void addNode(roundNode *front);
    void newRound();
public:
    tourney();
};

void tourney::australianDraw(Vector<player>& players){
    shuffle(players);
    shuffle(players);
    shuffle(players);
    std::ofstream nextRoundFile(_last->textFile, fstream::out);

    if(players.size() %2 == 1){
        nextRoundFile << players.get(players.size()-1).name << " vs BYE" << endl;
    }
    for(int i = 0; i < players.size(); i+=2){
        nextRoundFile << players.get(i).name << " vs " << players.get(i+1).name << endl;
    }
}
void tourney::startNew(){

    _name = getLine("Enter name of tournament:");
    _noParticipants = getInteger("Enter number of participants.");

    //Enter participant names
    createDirectoryPath("../" + _name);
    std::fstream participants("../" + _name + "/participants.txt", fstream::out);

    string newName = getLine("Enter name of first participant (Enter to stop): ");
    while(newName != ""){
        _participants.add(newName);
        participants << newName << endl;
        newName = getLine("Enter name of next participant (Enter to stop): ");
    }
    participants.close();
    //Text file with names of participants created.

    char choice = getChar("Enter a choice. (N)ext round, (D)raw, (Q)uit (E)nd Tournament");
    choice = toUpperCase(choice);
    while(choice != 'Q'){
        if(choice == 'N'){
            incrementRound();
        } else if (choice == 'D'){
            char typeDraw = getChar("Enter a choice. (A)ustralian Draw, or (K)ing of the Hill");
            typeDraw = toUpperCase(typeDraw);
            if(typeDraw == 'A'){
                Vector<player> shuffle = _last->playerOrder;
                australianDraw(shuffle);
            }
        } else if(choice == 'E'){
            //do nothing for now
        }

        choice = getChar("Enter a choice. (N)ext round, (D)raw, (Q)uit, (E)nd Tournament");
        choice = toUpperCase(choice);
    }
    cout << "Thanks for using! Remember, you can always open an existing tournament just by typing its name.";
}

void tourney::addNode(roundNode* newRound){
    if(_first == nullptr){
        _first = _last = newRound;
        newRound->prev = nullptr;
        //if first round, inititalize the first round's vector values
        for(player p : _participants){
            newRound->playerOrder.add(p);
        }
    } else{
        _last->next = newRound;
        newRound->prev = _last;
        _last = newRound;
        //Copy standings from last round
        newRound->playerOrder = newRound->prev->playerOrder;
    }
}

void tourney::incrementRound(){
    _roundNo++;
    cout << "Welcome to round " << _roundNo << endl;
    roundNode* newRound = new roundNode;
    newRound->textFile = "../" + _name + "/round" + integerToString(_roundNo) + ".txt";
    addNode(newRound);
}

tourney::tourney(){

    _first = nullptr;
    _last = nullptr;
    _noParticipants = 0;
    _roundNo = 0;
    cout << "Welcome to SP Beta!" << endl << "Head to http://sp.com for documentation on how to use SP."
         << endl << "Caution: Random round access support still under testing. Be careful before starting new round" << endl;
    _newTourney = getYesOrNo("New Tournament? (Y/N)");

    if(_newTourney){
        _roundNo    = 0;
        startNew();
    } else{
        //continueExisting();
    }
}

int main() {
    tourney tournament;
    return 0;
}
