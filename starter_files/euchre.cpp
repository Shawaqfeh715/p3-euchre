#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Player.hpp"
#include "Pack.hpp"
using namespace std;

int main(int argc, char **argv) {
    // 1. Check argument count
    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }

    // 2. getting basic arguments
    string pack_filename = argv[1];
    string shuffle_arg = argv[2];
    int points_to_win = stoi(argv[3]);

    // 3. Error check shuffle and points
    if ((shuffle_arg != "shuffle" && shuffle_arg != "noshuffle") || 
        (points_to_win < 1 || points_to_win > 100)) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }

    // 4. Verify player types and create them
    vector<Player*> players;
    for (int i = 4; i < 12; i += 2) {
        string name = argv[i];
        string type = argv[i+1];
        if (type != "Simple" && type != "Human") {
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
                 << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
                 << "NAME4 TYPE4" << endl;
            // Clean up any players already created before exiting
            for (Player* p : players) delete p;
            return 1;
        }
        players.push_back(Player_factory(name, type));
    }

    // 5. Check if pack file opens
    ifstream fin(pack_filename);
    if (!fin.is_open()) {
        cout << "Error opening " << pack_filename << endl;
        for (Player* p : players) delete p;
        return 1;
    }

    // Print arguments 
    for (int i = 0; i < argc; ++i) { cout << argv[i] << " "; }
    cout << endl;

    // TODO: Create game object and play
    
    // Cleanup
    for (Player* p : players) delete p;
    return 0;
}

class Game {
    public:
    Game(vector<Player*> players_in, Pack pack_in, int points_to_win_in,
         bool shuffle_in) : players(players_in), pack(pack_in),
         points_to_win(points_to_win_in), shuffle(shuffle_in),
         team1_score(0), team2_score(0), dealer_idx(0) {}

    void play() {
        int num_hand = 0;
        while (team1_score < points_to_win && team2_score < points_to_win) {
            cout << players[0]->get_name() << " and " << players[2]->get_name()
                 << " have " << team1_score << " points" << endl;
            cout << players[1]->get_name() << " and " << players[3]->get_name()
                 << " have " << team2_score << " points" << endl << endl;

            // preparing for the next hand
            dealer_idx = (dealer_idx + 1) % 4;
            num_hand++;
        }

        // announcing winner of the hand
        if (team1_score >= points_to_win) {
            cout << players[0]->get_name() << " and " << players[2]->get_name() << " win!" << endl;
        } else {
            cout << players[1]->get_name() << " and " << players[3]->get_name() << " win!" << endl;
        }
    }

}